#include <stdlib.h>
#include <app_preference.h>
#include <dlog.h>
#include "data.h"

static bundle *bundle_items = NULL;

static void bundle_load() {
  int len = 0, err = -1;
  char *str = NULL;
  if (PREFERENCE_ERROR_NONE != (err = preference_get_string(DATA_BUNDLE_RAW, &str))) {
    dlog_print(DLOG_ERROR, LOG_TAG, "[preference_get_string] failed: %d", err);
    return;
  }
  if (PREFERENCE_ERROR_NONE != (err = preference_get_int(DATA_BUNDLE_LEN, &len))) {
    dlog_print(DLOG_ERROR, LOG_TAG, "[preference_get_int] failed: %d", err);
    return;
  }
  bundle_raw *raw = (bundle_raw *) str;
  if (NULL == (bundle_items = bundle_decode(raw, len))) {
    dlog_print(DLOG_ERROR, LOG_TAG, "[bundle_decode] failed");
  }
  free(str);
}

static void bundle_save() {
  int len = 0, err = -1;
  bundle_raw *raw = NULL;
  if (BUNDLE_ERROR_NONE != (err = bundle_encode(bundle_items, &raw, &len))) {
    dlog_print(DLOG_ERROR, LOG_TAG, "[bundle_encode] bundle_save failed: %d", err);
    return;
  } else {
    if (PREFERENCE_ERROR_NONE != (err = preference_set_string(DATA_BUNDLE_RAW, (const char *) raw))) {
      dlog_print(DLOG_ERROR, LOG_TAG, "[preference_set_string] failed: %d", err);
    }
    if (PREFERENCE_ERROR_NONE != (err = preference_set_int(DATA_BUNDLE_LEN, len))) {
      dlog_print(DLOG_ERROR, LOG_TAG, "[preference_set_int] failed: %d", err);
    }
  }
}

void data_initialize() {
  if (NULL == bundle_items) {
    bool bundle_raw_exists = false, bundle_len_exists = false;
    preference_error_e err = -1;
    if (PREFERENCE_ERROR_NONE != (err = preference_is_existing(DATA_BUNDLE_RAW, &bundle_raw_exists))) {
      dlog_print(DLOG_ERROR, LOG_TAG, "[preference_is_existing] %s failed: %d", DATA_BUNDLE_RAW, err);
      return;
    }
    if (PREFERENCE_ERROR_NONE != (err = preference_is_existing(DATA_BUNDLE_LEN, &bundle_len_exists))) {
      dlog_print(DLOG_ERROR, LOG_TAG, "[preference_is_existing] %s failed: %d", DATA_BUNDLE_LEN, err);
      return;
    }
    if (bundle_raw_exists && bundle_len_exists) {
      bundle_load();
    } else {
      if (NULL == (bundle_items = bundle_create())) {
        dlog_print(DLOG_ERROR, LOG_TAG, "[bundle_create] failed");
      }
    }
  }
}

void data_terminate() {
  bundle_error_e err = -1;
  if (BUNDLE_ERROR_NONE != (err = bundle_free(bundle_items))) {
    dlog_print(DLOG_ERROR, LOG_TAG, "[bundle_free] failed: %d", err);
  }
  bundle_items = NULL;
}

void data_set(const char *key, const char *value) {
  // dlog_print(DLOG_INFO, LOG_TAG, "[data_set] key: %s, value: %s", key, value);
  bundle_error_e err = bundle_add_str(bundle_items, key, value);
  if (BUNDLE_ERROR_KEY_EXISTS == err) {
    data_delete(key);
    data_set(key, value);
  } else if (BUNDLE_ERROR_NONE != err) {
    dlog_print(DLOG_ERROR, LOG_TAG, "[bundle_add_str] failed: %d", err);
    return;
  }
  bundle_save();
}

char *data_get(const char *key) {
  char *value = "";
  bundle_error_e err = -1;
  if (BUNDLE_ERROR_NONE != (err = bundle_get_str(bundle_items, key, &value))) {
    dlog_print(DLOG_ERROR, LOG_TAG, "[bundle_get_str] failed: %d", err);
  }
  return value;
}

void data_iterate(bundle_iterate_cb_t callback, void *data) {
  bundle_iterate(bundle_items, callback, data);
}

void data_delete(const char *key) {
  bundle_error_e err = -1;
  if (BUNDLE_ERROR_NONE != (err = bundle_del(bundle_items, key))) {
    dlog_print(DLOG_ERROR, LOG_TAG, "[bundle_del] failed: %d", err);
    return;
  }
  bundle_save();
}
