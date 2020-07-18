#include "vipaccess.h"
#include "data.h"
#include "totp.h"

static Elm_Object_Item *settings_view_show(settings_view_context *context) {
  Elm_Object_Item *naviframe_item = elm_naviframe_item_push(context->naviframe,
      "Settings", NULL, NULL, context->box, NULL);
  evas_object_show(context->secret_label);
  evas_object_show(context->secret_entry);
  evas_object_show(context->save_button);
  evas_object_show(context->box);
  return naviframe_item;
}

settings_view_context *settings_view_create(Evas_Object *naviframe) {
  settings_view_context *context = calloc(1, sizeof(settings_view_context));
  context->naviframe = naviframe;
  context->show = &settings_view_show;

  /* Box */
  context->box = elm_box_add(context->naviframe);
  evas_object_size_hint_weight_set(context->box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(context->box, EVAS_HINT_FILL, EVAS_HINT_FILL);

  /* Label: Secret */
  context->secret_label = elm_label_add(context->box);
  evas_object_size_hint_weight_set(context->secret_label, EVAS_HINT_EXPAND, 0.0);
  evas_object_size_hint_align_set(context->secret_label, EVAS_HINT_FILL, EVAS_HINT_FILL);
  elm_object_text_set(context->secret_label, "Secret");
  elm_box_pack_end(context->box, context->secret_label);

  /* Entry: Secret */
  context->secret_entry = elm_entry_add(context->box);
  evas_object_size_hint_weight_set(context->secret_entry, EVAS_HINT_EXPAND, 0.3);
  evas_object_size_hint_align_set(context->secret_entry, EVAS_HINT_FILL, 0.0);
  const char *secret = data_get(DATA_SECRET);
  if (strlen(secret) > 0) {
    elm_entry_entry_set(context->secret_entry, secret);
  } else {
    elm_object_part_text_set(context->secret_entry, "elm.guide", "No secret set!");
  }
  elm_entry_editable_set(context->secret_entry, EINA_FALSE);
  elm_entry_text_style_user_push(context->secret_entry, "DEFAULT='color=" COLOR_GREEN "'");
  elm_box_pack_end(context->box, context->secret_entry);

  /* Button: Save */
  context->save_button = elm_button_add(context->box);
  evas_object_size_hint_weight_set(context->save_button, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(context->save_button, EVAS_HINT_FILL, 1.0);
  elm_object_text_set(context->save_button, "Save");
  elm_object_disabled_set(context->save_button, !validate_secret(elm_entry_entry_get(context->secret_entry)));
  elm_box_pack_end(context->box, context->save_button);

  return context;
}
