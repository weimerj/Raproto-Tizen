/*
 * settings_menu.c
 *
 *  Created on: Feb 15, 2021
 *      Author: weimer
 */


#include "settings_menu.h"



static void
update_settings_user(char *key, app_data_s *ad){
	int err;
	char *str;
	if ((err = bundle_get_str(ad->settings, key, &str)) != BUNDLE_ERROR_NONE) return error_msg(err, __func__, "get str");
	if (strlen(str) > 0) {
		if ((err = app_control_add_extra_data(ad->service.control, key, str)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "add data");
	}
}


void
update_settings(const char *event_name, bundle *settings, void *data){
	app_data_s *ad = (app_data_s*)data;
	int err;
	int *error;
	int *logging;
	size_t b_size;
	bool init_flag;

	if (ad->settings == NULL) {

		init_flag = true;
		ad->settings = bundle_dup(settings);
		//dlog_print(DLOG_INFO, LOG_TAG, "starting GUI");
		create_base_gui(ad); // do this here once settings are created

		update_settings_user(RAPROTO_SETTING_DEVICE_ID, ad);
		update_settings_user(RAPROTO_SETTING_MQTT_BROKER, ad);
		update_settings_user(RAPROTO_SETTING_MQTT_USERNAME, ad);
		update_settings_user(RAPROTO_SETTING_MQTT_PASSWORD, ad);
		update_settings_user(RAPROTO_SETTING_MQTT_CONFIG_PUB_TOPIC, ad);
		update_settings_user(RAPROTO_SETTING_MQTT_CONFIG_SUB_TOPIC, ad);
	} else {
		init_flag = false;
		if ((err = bundle_free(ad->settings)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "bundle free");
		ad->settings = bundle_dup(settings);
	}

	if ((err = bundle_get_byte(ad->settings, RAPROTO_SETTING_ERROR, (void**)&error, &b_size)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get error byte");
	if ((err = bundle_get_byte(ad->settings, RAPROTO_SETTING_LOGGING, (void**)&logging, &b_size)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get logging byte");

	switch(*error) {
		case (RAPROTO_SETTING_ERROR_TRUE):
			ad->service.error = true;
			break;
		case (RAPROTO_SETTING_ERROR_FALSE):
			ad->service.error = false;
			break;
		default:
			error_msg(BUNDLE_ERROR_INVALID_PARAMETER, __func__, "unknown error byte");
	}

	switch(*logging){
		case (RAPROTO_SETTING_LOGGING_TRUE):
			ad->service.logging = true;
			break;
		case (RAPROTO_SETTING_LOGGING_FALSE):
			ad->service.logging = false;
			break;
		default:
			error_msg(BUNDLE_ERROR_INVALID_PARAMETER, __func__, "unknown logging byte");
	}

	if (ad->frame_main.gl != NULL) update_frame(ad->frame_main.gl, NUM_OF_MAIN_MENU_ITEMS);
	if (ad->frame_settings.gl != NULL) update_frame(ad->frame_settings.gl, NUM_OF_SETTINGS_MENU_ITEMS);
	if (ad->frame_about.gl != NULL) update_frame(ad->frame_about.gl, NUM_OF_ABOUT_MENU_ITEMS);
	if (ad->frame_mqtt.gl != NULL) update_frame(ad->frame_mqtt.gl, NUM_OF_MQTT_MENU_ITEMS);
	if (ad->frame_config.gl != NULL) update_frame(ad->frame_config.gl, NUM_OF_CONFIG_MENU_ITEMS);
	update_theme(ad);

	dlog_print(DLOG_INFO, LOG_TAG, "settings updated");
}




/*
 * Get content of settings menu
 */
static Evas_Object*
get_settings_menu_content(void *data, Evas_Object *obj, const char *part)
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
		case (SETTINGS_CONFIG):
			if ((err = bundle_get_str(lid->settings, RAPROTO_SETTING_LAST_UPDATE, &str)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "config status");
			bg = get_genlist_item_2text(obj,"Configuration",str);
			break;
		case (SETTINGS_MQTT):
			if ((err = bundle_get_str(lid->settings, RAPROTO_SETTING_MQTT_BROKER, &str)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "broker");
			bg = get_genlist_item_2text(obj,"MQTT", str);
			break;
		default:
			return NULL;
	}

	get_background_for_state(bg,lid->service);

	return bg;
}


/*
 * Get title of the settings view
 */
static char *
get_settings_menu_title(void *data, Evas_Object *obj, const char *part)
{
	return strdup("Settings");
}



static void
on_mqtt_cb(void *data, Evas_Object *obj, void *event_info) {
	app_data_s *ad = (app_data_s*)data;
	return create_mqtt_menu(ad);
}

static void
on_config_cb(void *data, Evas_Object *obj, void *event_info) {
	app_data_s *ad = (app_data_s*)data;
	return create_config_menu(ad);
}


void
create_settings_menu(app_data_s *ad) {

	Evas_Smart_Cb func[NUM_OF_SETTINGS_MENU_ITEMS];
	func[SETTINGS_CONFIG] = on_config_cb;
	func[SETTINGS_MQTT] = on_mqtt_cb;

	menu_data_s menu;
	menu.title_text = get_settings_menu_title;
	menu.item_content = get_settings_menu_content;
	menu.num_items = NUM_OF_SETTINGS_MENU_ITEMS;
	menu.func = func;

	Evas_Object *frame = create_frame(&(ad->frame_settings), &menu, ad);
	elm_naviframe_item_push(ad->nf, NULL, NULL, NULL, frame, "empty");
}
