#ifndef VIPACCESS_INC_VIPACCESS_H_
#define VIPACCESS_INC_VIPACCESS_H_

#include <Elementary.h>
#include <glib.h>
#include <stdbool.h>

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "vipaccess"

#if !defined(PACKAGE)
#define PACKAGE "com.github.wolandmaster.vipaccess"
#endif

#define COLOR_RED "#CB0000"
#define COLOR_GREEN "#68B107"

#define DATA_SECRET "secret"

typedef struct secret_view_context secret_view_context;
struct secret_view_context {
  Evas_Object *naviframe;
  Evas_Object *box;
  Evas_Object *secret_entry;

  Elm_Object_Item *(*show)(secret_view_context *);
};

typedef struct settings_view_context settings_view_context;
struct settings_view_context {
  Evas_Object *naviframe;
  Evas_Object *box;
  Evas_Object *secret_label;
  Evas_Object *secret_entry;
  Evas_Object *save_button;

  secret_view_context *secret_context;

  Elm_Object_Item *(*show)(settings_view_context *);
};

typedef struct main_view_context main_view_context;
struct main_view_context {
  Evas_Object *naviframe;
  Evas_Object *box;
  Evas_Object *code_label;
  Evas_Object *time_progressbar;
  Evas_Object *settings_button;

  char *secret;
  Ecore_Timer *timer;
  settings_view_context *settings_context;

  Elm_Object_Item *(*show)(main_view_context *);
};

typedef struct app_context app_context;
struct app_context {
  Evas_Object *window;
  Evas_Object *conform;
  Evas_Object *naviframe;

  bool quick_start;

  main_view_context *main_context;
};

#endif /* VIPACCESS_INC_VIPACCESS_H_ */
