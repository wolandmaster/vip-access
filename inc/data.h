#ifndef VIPACCESS_INC_DATA_H_
#define VIPACCESS_INC_DATA_H_

#include <bundle.h>

#define DATA_BUNDLE_RAW "vipaccess-bundle-raw"
#define DATA_BUNDLE_LEN "vipaccess-bundle-len"

void data_initialize();
void data_terminate();
void data_set(const char *, const char *);
char *data_get(const char *);
void data_iterate(bundle_iterate_cb_t, void *);
void data_delete(const char *);

#endif //VIPACCESS_INC_DATA_H_
