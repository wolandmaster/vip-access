#ifndef VIPACCESS_INC_TOTP_H_
#define VIPACCESS_INC_TOTP_H_

#define TOTP_REFRESH_MILLIS 30000

Eina_Bool validate_secret(const char *);
unsigned long long get_epoch();
double get_progress(unsigned long long);
void decode_secret(unsigned char *);
unsigned int totp_generate(char *, unsigned long long);

#endif //VIPACCESS_INC_TOTP_H_
