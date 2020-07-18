#include "vipaccess.h"

static Elm_Entry_Filter_Accept_Set secret_entry_accepted_characters = {
    .accepted = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=",
    .rejected = NULL
};

static Elm_Object_Item *secret_view_show(secret_view_context *context) {
  Elm_Object_Item *naviframe_item = elm_naviframe_item_push(context->naviframe,
      "", NULL, NULL, context->box, NULL);
  elm_naviframe_item_title_enabled_set(naviframe_item, EINA_FALSE, EINA_FALSE);
  evas_object_show(context->secret_entry);
  evas_object_show(context->box);
  return naviframe_item;
}

secret_view_context *secret_view_create(Evas_Object *naviframe) {
  secret_view_context *context = calloc(1, sizeof(secret_view_context));
  context->naviframe = naviframe;
  context->show = &secret_view_show;

  /* Box */
  context->box = elm_box_add(context->naviframe);
  evas_object_size_hint_weight_set(context->box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(context->box, EVAS_HINT_FILL, EVAS_HINT_FILL);

  /* Entry: Secret */
  context->secret_entry = elm_entry_add(context->box);
  evas_object_size_hint_weight_set(context->secret_entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(context->secret_entry, EVAS_HINT_FILL, EVAS_HINT_FILL);
  elm_object_part_text_set(context->secret_entry, "elm.guide", "Secret");
  elm_entry_input_panel_return_key_type_set(context->secret_entry, ELM_INPUT_PANEL_RETURN_KEY_TYPE_DONE);
  elm_entry_input_panel_return_key_autoenabled_set(context->secret_entry, EINA_TRUE);
  elm_entry_markup_filter_append(context->secret_entry, elm_entry_filter_accept_set, &secret_entry_accepted_characters);
  elm_entry_input_panel_layout_set(context->secret_entry, ELM_INPUT_PANEL_LAYOUT_PASSWORD);
  elm_entry_prediction_allow_set(context->secret_entry, EINA_FALSE);
  elm_entry_cursor_line_end_set(context->secret_entry);
  elm_box_pack_end(context->box, context->secret_entry);

  return context;
}
