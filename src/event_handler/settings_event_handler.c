#include "vipaccess.h"
#include "data.h"
#include "totp.h"
#include "main_view.h"
#include "secret_view.h"
#include "secret_event_handler.h"

Eina_Bool settings_naviframe_pop_cb(void *data, Elm_Object_Item *naviframe_item EINA_UNUSED) {
  main_view_context *main_context = data;
  free(main_context->settings_context);
  main_context->settings_context = NULL;
  return EINA_TRUE;
}

void settings_secret_entry_clicked_cb(main_view_context *context, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED) {
  context->settings_context->secret_context = secret_view_create(context->naviframe);
  evas_object_smart_callback_add(context->settings_context->secret_context->secret_entry, "activated", (Evas_Smart_Cb) secret_secret_entry_done_cb, context);
  elm_entry_entry_set(context->settings_context->secret_context->secret_entry, elm_entry_entry_get(context->settings_context->secret_entry));
  Elm_Object_Item *naviframe_item = context->settings_context->secret_context->show(context->settings_context->secret_context);
  elm_naviframe_item_pop_cb_set(naviframe_item, secret_naviframe_pop_cb, context);
}

void settings_save_button_clicked_cb(main_view_context *context, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED) {
  const char *secret = elm_entry_entry_get(context->settings_context->secret_entry);
  data_set(DATA_SECRET, secret);
  context->secret = strdup(secret);
  decode_secret((unsigned char *) context->secret);
  code_text_set(context->code_label, totp_generate(context->secret, get_epoch()));
  elm_naviframe_item_pop(context->naviframe);
}
