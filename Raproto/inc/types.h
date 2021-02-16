/*
 * types.h
 *
 *  Created on: Feb 15, 2021
 *      Author: weimer
 */

#ifndef TYPES_H_
#define TYPES_H_

#include <tizen.h>
#include <Elementary.h>
#include <app_control.h>
#include <efl_extension.h>
#include <bundle.h>



typedef struct menu_data{
	Elm_Gen_Item_Text_Get_Cb title_text;
	Elm_Gen_Item_Content_Get_Cb item_content;
	Elm_Gen_Item_Del_Cb item_delete;
	int num_items;
	Evas_Smart_Cb *func;
} menu_data_s;


typedef struct service_data{
	bool error;
	bool logging;
	bool reset;
	bool reboot;
	app_control_h control;
	time_t t;
	int reset_taps;
} service_data_s;



typedef struct list_item_data {
	int index;
	bundle *settings;
	service_data_s *service;
} list_item_data_s;



typedef struct frame_data{
	Evas_Object *gl;
	Evas_Object *bg;
} frame_data_s;

typedef struct app_data {
	Evas_Object *win;
	Evas_Object *layout;
	Evas_Object *bg;
	Evas_Object *conform;
	Evas_Object *nf;
	Eext_Circle_Surface *circle_surface;
	frame_data_s frame_main;
	frame_data_s frame_settings;
	frame_data_s frame_about;
	frame_data_s frame_mqtt;
	frame_data_s frame_config;
	bundle *settings;
	service_data_s service;
} app_data_s;




#endif /* TYPES_H_ */
