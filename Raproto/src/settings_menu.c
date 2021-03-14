/*
 * settings_menu.c
 *
 *  Created on: Feb 15, 2021
 *      Author: weimer
 */


#include "settings_menu.h"




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
			if ((err = bundle_get_str(*(lid->settings), RAPROTO_SETTING_LAST_UPDATE, &str)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "config status");
			bg = get_genlist_item_2text(obj,"Configuration",str);
			break;
		case (SETTINGS_MQTT):
			if ((err = bundle_get_str(*(lid->settings), RAPROTO_SETTING_MQTT_BROKER, &str)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "broker");
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
