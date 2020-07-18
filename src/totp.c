/*
 * TOTP: Time-Based One-Time Password Algorithm
 *
 * Based on https://gist.github.com/syzdek/eba233ca33e1b5a45a99
 * Copyright (c) 2015, David M. Syzdek <david@syzdek.net>
 */

#include <sys/time.h>
#include <string.h>
#include <openssl/hmac.h>
#include <eina-1/eina/eina_types.h>
#include "totp.h"

static const signed char base32_vals[256] = {
//    This map cheats and interprets:
//       - the numeral zero as the letter "O" as in oscar
//       - the numeral one as the letter "L" as in lima
//       - the numeral eight as the letter "B" as in bravo
//  00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x00
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x10
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x20
    14, 11, 26, 27, 28, 29, 30, 31, +1, -1, -1, -1, -1, +0, -1, -1, // 0x30
    -1, +0, +1, +2, +3, +4, +5, +6, +7, +8, +9, 10, 11, 12, 13, 14, // 0x40
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, // 0x50
    -1, +0, +1, +2, +3, +4, +5, +6, +7, +8, +9, 10, 11, 12, 13, 14, // 0x60
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, -1, -1, -1, -1, // 0x70
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x80
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x90
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xA0
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xB0
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xC0
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xD0
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xE0
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xF0
};

void decode_secret(unsigned char *secret) {
  int key_len = 0;
  size_t len = strlen((char *) secret);

  for (unsigned int pos = 0; pos <= (len - 8); pos += 8) {
    // MSB is Most Significant Bits  (0x80 == 10000000 ~= MSB)
    // MB is middle bits             (0x7E == 01111110 ~= MB)
    // LSB is Least Significant Bits (0x01 == 00000001 ~= LSB)

    // byte 0
    secret[key_len + 0] = (base32_vals[secret[pos + 0]] << 3) & 0xF8; // 5 MSB
    secret[key_len + 0] |= (base32_vals[secret[pos + 1]] >> 2) & 0x07; // 3 LSB
    if (secret[pos + 2] == '=') {
      key_len += 1;
      break;
    };

    // byte 1
    secret[key_len + 1] = (base32_vals[secret[pos + 1]] << 6) & 0xC0; // 2 MSB
    secret[key_len + 1] |= (base32_vals[secret[pos + 2]] << 1) & 0x3E; // 5  MB
    secret[key_len + 1] |= (base32_vals[secret[pos + 3]] >> 4) & 0x01; // 1 LSB
    if (secret[pos + 4] == '=') {
      key_len += 2;
      break;
    };

    // byte 2
    secret[key_len + 2] = (base32_vals[secret[pos + 3]] << 4) & 0xF0; // 4 MSB
    secret[key_len + 2] |= (base32_vals[secret[pos + 4]] >> 1) & 0x0F; // 4 LSB
    if (secret[pos + 5] == '=') {
      key_len += 3;
      break;
    };

    // byte 3
    secret[key_len + 3] = (base32_vals[secret[pos + 4]] << 7) & 0x80; // 1 MSB
    secret[key_len + 3] |= (base32_vals[secret[pos + 5]] << 2) & 0x7C; // 5  MB
    secret[key_len + 3] |= (base32_vals[secret[pos + 6]] >> 3) & 0x03; // 2 LSB
    if (secret[pos + 7] == '=') {
      key_len += 4;
      break;
    };

    // byte 4
    secret[key_len + 4] = (base32_vals[secret[pos + 6]] << 5) & 0xE0; // 3 MSB
    secret[key_len + 4] |= (base32_vals[secret[pos + 7]] >> 0) & 0x1F; // 5 LSB
    key_len += 5;
  };
  secret[key_len] = 0;
}

Eina_Bool validate_secret(const char *secret) {
  size_t len = strlen((char *) secret);

  if (len == 0 || (((len & 0xF) != 0) && ((len & 0xF) != 8))) {
    return EINA_FALSE;
  }
  for (unsigned int pos = 0; (pos < len); pos++) {
    if (base32_vals[(unsigned char) secret[pos]] == -1) {
      return EINA_FALSE;
    }
    if (secret[pos] == '=') {
      if (((pos & 0xF) == 0) || ((pos & 0xF) == 8)) {
        return EINA_FALSE;
      }
      if ((len - pos) > 6) {
        return EINA_FALSE;
      }
      switch (pos % 8) {
        case 2:
        case 4:
        case 5:
        case 7:
          break;
        default:
          return EINA_FALSE;
      }
      for (; (pos < len); pos++) {
        if (secret[pos] != '=') {
          return EINA_FALSE;
        }
      }
    }
  }
  return EINA_TRUE;
}

unsigned long long get_epoch() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (unsigned long long) (tv.tv_sec) * 1000 + (unsigned long long) (tv.tv_usec) / 1000;
}

double get_progress(unsigned long long epoch) {
  unsigned short remaining = TOTP_REFRESH_MILLIS - epoch % TOTP_REFRESH_MILLIS;
  return remaining / (double) TOTP_REFRESH_MILLIS;
}

unsigned int totp_generate(char *decoded_secret, unsigned long long epoch) {
  unsigned long long time = epoch / TOTP_REFRESH_MILLIS;

  // converts time to big endian if system is little endian
  unsigned int endianness = 0xdeadbeef;
  if ((*(const unsigned char *) &endianness) == 0xef) {
    time = ((time & 0x00000000ffffffff) << 32) | ((time & 0xffffffff00000000) >> 32);
    time = ((time & 0x0000ffff0000ffff) << 16) | ((time & 0xffff0000ffff0000) >> 16);
    time = ((time & 0x00ff00ff00ff00ff) << 8) | ((time & 0xff00ff00ff00ff00) >> 8);
  };

  // determines hash
  unsigned char *hmac_result = HMAC(EVP_sha1(), decoded_secret, (int) strlen((const char *) decoded_secret),
      (const unsigned char *) &time, sizeof(time), NULL, 0);

  // dynamically truncates hash
  unsigned long long offset = hmac_result[19] & 0x0f;
  unsigned int bin_code = (hmac_result[offset] & 0x7f) << 24
      | (hmac_result[offset + 1] & 0xff) << 16
      | (hmac_result[offset + 2] & 0xff) << 8
      | (hmac_result[offset + 3] & 0xff);

  // truncates code to 6 digits
  return bin_code % 1000000;
}
