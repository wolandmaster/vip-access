#include <dlog.h>
#include "vipaccess.h"
#include "main_event_handler.h"

static Elm_Object_Item *main_view_show(main_view_context *context) {
  Elm_Object_Item *naviframe_item = elm_naviframe_item_push(context->naviframe,
      "VIP Access", NULL, NULL, context->box, NULL);
  progress_timer_cb(context);
  if (!context->timer) {
    context->timer = ecore_timer_add(0.25, progress_timer_cb, context);
  }
  evas_object_show(context->code_label);
  evas_object_show(context->time_progressbar);
  evas_object_show(context->settings_button);
  evas_object_show(context->box);
  return naviframe_item;
}

void code_text_set(Evas_Object *obj, unsigned int code) {
  char buf[128];
  snprintf(buf, sizeof(buf), "<font_size=60><align=center>%06u</align></font_size>", code);
  elm_object_text_set(obj, buf);
}

main_view_context *main_view_create(Evas_Object *naviframe) {
  main_view_context *context = calloc(1, sizeof(main_view_context));
  context->naviframe = naviframe;
  context->show = &main_view_show;

  /* Box */
  context->box = elm_box_add(context->naviframe);
  evas_object_size_hint_weight_set(context->box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(context->box, EVAS_HINT_FILL, EVAS_HINT_FILL);

  /* Label: Code */
  context->code_label = elm_label_add(context->box);
  evas_object_size_hint_weight_set(context->code_label, EVAS_HINT_EXPAND, 0.3);
  evas_object_size_hint_align_set(context->code_label, EVAS_HINT_FILL, 1.0);
  elm_box_pack_end(context->box, context->code_label);

  /* Progressbar: Time */
  context->time_progressbar = elm_progressbar_add(context->box);
  evas_object_size_hint_weight_set(context->time_progressbar, EVAS_HINT_EXPAND, 0.7);
  evas_object_size_hint_align_set(context->time_progressbar, EVAS_HINT_FILL, 0.0);
  elm_box_pack_end(context->box, context->time_progressbar);

  /* Button: Settings */
  context->settings_button = elm_button_add(context->box);
  evas_object_size_hint_weight_set(context->settings_button, EVAS_HINT_EXPAND, 0.0);
  evas_object_size_hint_align_set(context->settings_button, EVAS_HINT_FILL, EVAS_HINT_FILL);
  elm_object_text_set(context->settings_button, "Settings");
  evas_object_smart_callback_add(context->settings_button, "clicked", (Evas_Smart_Cb) main_settings_button_clicked_cb, context);
  elm_box_pack_end(context->box, context->settings_button);

  return context;
}
