#include <string.h>
#include "vipaccess.h"
#include "totp.h"

static const char *strremove(char *str, const char *sub) {
  char *p = NULL, *q = NULL, *r = NULL;
  if ((q = r = strstr(str, sub)) != NULL) {
    size_t len = strlen(sub);
    while ((r = strstr(p = r + len, sub)) != NULL) {
      memmove(q, p, r - p);
      q += r - p;
    }
    memmove(q, p, strlen(p) + 1);
  }
  return str;
}

Eina_Bool secret_naviframe_pop_cb(void *data, Elm_Object_Item *naviframe_item EINA_UNUSED) {
  main_view_context *main_context = data;
  free(main_context->settings_context->secret_context);
  main_context->settings_context->secret_context = NULL;
  return EINA_TRUE;
}

void secret_secret_entry_done_cb(main_view_context *context, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED) {
  const char *secret = strremove(strdup(elm_entry_entry_get(context->settings_context->secret_context->secret_entry)), "<br/>");
  elm_entry_entry_set(context->settings_context->secret_entry, secret);
  elm_entry_text_style_user_pop(context->settings_context->secret_entry);
  if (validate_secret(secret)) {
    elm_entry_text_style_user_push(context->settings_context->secret_entry, "DEFAULT='color=" COLOR_GREEN "'");
    elm_object_disabled_set(context->settings_context->save_button, EINA_FALSE);
  } else {
    elm_entry_text_style_user_push(context->settings_context->secret_entry, "DEFAULT='color=" COLOR_RED "'");
    elm_object_disabled_set(context->settings_context->save_button, EINA_TRUE);
  }
  elm_naviframe_item_pop(context->naviframe);
}
