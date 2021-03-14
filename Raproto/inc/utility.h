/*
 * utility.h
 *
 *  Created on: Feb 15, 2021
 *      Author: weimer
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include "main_menu.h"

#include "types.h"
#include "definitions.h"
#include <Elementary.h>
#include <app.h>
#include <dlog.h>

void update_settings(const char *event_name, bundle *settings, void *data);
void update_theme(app_data_s *ad);
void update_frame(Evas_Object *gl, int num_entries);

void error_msg(int rc, char const *caller_name, char const *id);
Evas_Object* get_genlist_item_2text1check(Evas_Object *obj, const char *text, const char *subtext, Evas_Object **check);
Evas_Object* get_genlist_item_1text(Evas_Object *obj, const char *text);
Evas_Object* get_genlist_item_2text(Evas_Object *obj, const char *text, const char *subtext);
void get_background_for_state(Evas_Object *bg, service_data_s *service);

Eina_Bool win_delete_request_cb(void *data, Elm_Object_Item *it);

void create_text_display(app_data_s *ad, const char *msg);
void create_entry(app_data_s *ad, Elm_Input_Panel_Layout layout, char *initial_entry, Evas_Smart_Cb callback);
Evas_Object* create_frame(frame_data_s *frame, menu_data_s *md, app_data_s *ad);


#endif /* UTILITY_H_ */
