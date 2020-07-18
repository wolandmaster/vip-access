#include "vipaccess.h"
#include "main_view.h"
#include "settings_view.h"
#include "settings_event_handler.h"
#include "totp.h"

Eina_Bool progress_timer_cb(void *data) {
  main_view_context *context = data;
  unsigned long long epoch = get_epoch();
  double progress = get_progress(epoch);
  double last_progress = elm_progressbar_value_get(context->time_progressbar);
  elm_progressbar_value_set(context->time_progressbar, progress);
  if (progress > last_progress) {
    if (strlen(context->secret) > 0) {
      code_text_set(context->code_label, totp_generate(context->secret, epoch));
    } else {
      code_text_set(context->code_label, 0);
    }
  }
  return ECORE_CALLBACK_RENEW;
}

void main_settings_button_clicked_cb(main_view_context *context, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED) {
  context->settings_context = settings_view_create(context->naviframe);
  evas_object_smart_callback_add(context->settings_context->secret_label, "clicked", (Evas_Smart_Cb) settings_secret_entry_clicked_cb, context);
  evas_object_smart_callback_add(context->settings_context->secret_entry, "clicked", (Evas_Smart_Cb) settings_secret_entry_clicked_cb, context);
  evas_object_smart_callback_add(context->settings_context->save_button, "clicked", (Evas_Smart_Cb) settings_save_button_clicked_cb, context);
  Elm_Object_Item *naviframe_item = context->settings_context->show(context->settings_context);
  elm_naviframe_item_pop_cb_set(naviframe_item, settings_naviframe_pop_cb, context);
}
