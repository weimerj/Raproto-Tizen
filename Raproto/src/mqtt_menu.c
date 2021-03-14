/*
 * mqtt_menu.c
 *
 *  Created on: Feb 15, 2021
 *      Author: weimer
 */


#include "mqtt_menu.h"





/*
 * Get content of mqtt menu
 */
static Evas_Object*
get_mqtt_menu_content(void *data, Evas_Object *obj, const char *part)
{
	list_item_data_s *lid = (list_item_data_s *)data;
	Evas_Object *bg = NULL;
	int err;
	char *str;

	//dlog_print(DLOG_INFO, LOG_TAG, "%d : %s", lid->index, part);

	// only using elm.swallow.content -- anything else is an error.
	if (strcmp("elm.swallow.content", part)){
		error_msg(EVAS_LOAD_ERROR_GENERIC, __func__, part);
		return NULL;
	}

	switch (lid->index) {
		case (MQTT_BROKER):
			if ((err = bundle_get_str(*(lid->settings), RAPROTO_SETTING_MQTT_BROKER, &str)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "broker");
			bg = get_genlist_item_2text(obj,"Broker Address", str);
			break;
		case (MQTT_USERNAME):
			if ((err = bundle_get_str(*(lid->settings), RAPROTO_SETTING_MQTT_USERNAME, &str)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "username");
			bg = get_genlist_item_2text(obj,"Username", str);
			break;
		case (MQTT_PASSWORD):
			bg = get_genlist_item_2text(obj,"Password", NULL);
			break;
		default:
			return NULL;
	}

	get_background_for_state(bg,lid->service);

	return bg;
}



/*
 * Get title of the mqtt view
 */
static char *
get_mqtt_menu_title(void *data, Evas_Object *obj, const char *part)
{
	return strdup("MQTT Settings");
}



void
return_publish_topic(void *data, Evas_Object *entry, void *event_info){
	app_data_s *ad = (app_data_s*)data;
	int err;

	if ((err = app_control_remove_extra_data(ad->service.control, RAPROTO_SETTING_MQTT_CONFIG_PUB_TOPIC)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "remove extra data");
	if ((err = app_control_add_extra_data(ad->service.control, RAPROTO_SETTING_MQTT_CONFIG_PUB_TOPIC, elm_entry_entry_get(entry))) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "extra data");

	launch_service_task(RAPROTO_TASK_REQUEST_SETTINGS_TRANSMIT, ad);

	elm_naviframe_item_pop(ad->nf);

	return;
}


void
return_subscribe_topic(void *data, Evas_Object *entry, void *event_info){
	app_data_s *ad = (app_data_s*)data;
	int err;

	if ((err = app_control_remove_extra_data(ad->service.control, RAPROTO_SETTING_MQTT_CONFIG_SUB_TOPIC)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "remove extra data");
	if ((err = app_control_add_extra_data(ad->service.control, RAPROTO_SETTING_MQTT_CONFIG_SUB_TOPIC, elm_entry_entry_get(entry))) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "extra data");

	launch_service_task(RAPROTO_TASK_REQUEST_SETTINGS_TRANSMIT, ad);

	elm_naviframe_item_pop(ad->nf);

	return;
}


static void
return_password(void *data, Evas_Object *entry, void *event_info){
	app_data_s *ad = (app_data_s*)data;
	int err;

	if ((err = app_control_remove_extra_data(ad->service.control, RAPROTO_SETTING_MQTT_PASSWORD)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "remove extra data");
	if ((err = app_control_add_extra_data(ad->service.control, RAPROTO_SETTING_MQTT_PASSWORD, elm_entry_entry_get(entry))) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "extra data");

	launch_service_task(RAPROTO_TASK_REQUEST_SETTINGS_TRANSMIT, ad);

	elm_naviframe_item_pop(ad->nf);

	return;
}


static void
return_username(void *data, Evas_Object *entry, void *event_info){
	app_data_s *ad = (app_data_s*)data;
	int err;

	if ((err = app_control_remove_extra_data(ad->service.control, RAPROTO_SETTING_MQTT_USERNAME)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "remove extra data");
	if ((err = app_control_add_extra_data(ad->service.control, RAPROTO_SETTING_MQTT_USERNAME, elm_entry_entry_get(entry))) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "extra data");

	launch_service_task(RAPROTO_TASK_REQUEST_SETTINGS_TRANSMIT, ad);

	elm_naviframe_item_pop(ad->nf);

	return;
}


static void
return_broker(void *data, Evas_Object *entry, void *event_info){
	app_data_s *ad = (app_data_s*)data;
	int err;

	if ((err = app_control_remove_extra_data(ad->service.control, RAPROTO_SETTING_MQTT_BROKER)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "remove extra data");
	if ((err = app_control_add_extra_data(ad->service.control, RAPROTO_SETTING_MQTT_BROKER, elm_entry_entry_get(entry))) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "extra data");

	launch_service_task(RAPROTO_TASK_REQUEST_SETTINGS_TRANSMIT, ad);


	elm_naviframe_item_pop(ad->nf);

	return;
}



static void
on_mqtt_address_cb(void *data, Evas_Object *obj, void *event_info) {
	app_data_s *ad = (app_data_s*)data;
	char *broker;
	bundle_get_str(ad->settings, RAPROTO_SETTING_MQTT_BROKER, &broker);
	return create_entry(ad, ELM_INPUT_PANEL_LAYOUT_URL, broker, return_broker);
}

static void
on_mqtt_username_cb(void *data, Evas_Object *obj, void *event_info) {
	app_data_s *ad = (app_data_s*)data;
	char *username;
	bundle_get_str(ad->settings, RAPROTO_SETTING_MQTT_USERNAME, &username);
	return create_entry(ad, ELM_INPUT_PANEL_LAYOUT_PASSWORD, username, return_username);
}

static void
on_mqtt_password_cb(void *data, Evas_Object *obj, void *event_info) {
	app_data_s *ad = (app_data_s*)data;
	char *password;
	bundle_get_str(ad->settings, RAPROTO_SETTING_MQTT_PASSWORD, &password);
	return create_entry(ad, ELM_INPUT_PANEL_LAYOUT_PASSWORD, password, return_password);
}


void
create_mqtt_menu(app_data_s *ad) {
	Evas_Smart_Cb func[NUM_OF_MQTT_MENU_ITEMS];
	func[MQTT_BROKER] = on_mqtt_address_cb;
	func[MQTT_USERNAME] = on_mqtt_username_cb;
	func[MQTT_PASSWORD] = on_mqtt_password_cb;

	menu_data_s menu;
	menu.title_text = get_mqtt_menu_title;
	menu.item_content = get_mqtt_menu_content;
	menu.num_items = NUM_OF_MQTT_MENU_ITEMS;
	menu.func = func;

	Evas_Object *frame = create_frame(&(ad->frame_mqtt), &menu, ad);
	elm_naviframe_item_push(ad->nf, NULL, NULL, NULL, frame, "empty");
}

