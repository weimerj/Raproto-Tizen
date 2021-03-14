/*
 * config_menu.c
 *
 *  Created on: Feb 15, 2021
 *      Author: weimer
 */


#include "config_menu.h"





/*
 * Get content of config menu
 */
static Evas_Object*
get_config_menu_content(void *data, Evas_Object *obj, const char *part)
{
	list_item_data_s *lid = (list_item_data_s *)data;
	Evas_Object *bg = NULL;
	int err;
	char *str;

	//dlog_print(DLOG_INFO, LOG_TAG, "Config %d : %s", lid->index, part);

	// only using elm.swallow.content -- anything else is an error.
	if (strcmp("elm.swallow.content", part)){
		error_msg(EVAS_LOAD_ERROR_GENERIC, __func__, part);
		return NULL;
	}

	switch (lid->index) {
		case (CONFIG_UPDATE):
			if ((err = bundle_get_str(*(lid->settings), RAPROTO_SETTING_CONFIG_STATUS, &str)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "config status");
			bg = get_genlist_item_2text(obj,"Update", str);
			break;
		case (CONFIG_SUBSCRIBE):
			if ((err = bundle_get_str(*(lid->settings), RAPROTO_SETTING_MQTT_CONFIG_SUB_TOPIC, &str)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "sub topic");
			bg = get_genlist_item_2text(obj,"Subscribe Topic", str);
			break;
		case (CONFIG_PUBLISH):
			if ((err = bundle_get_str(*(lid->settings), RAPROTO_SETTING_MQTT_CONFIG_PUB_TOPIC, &str)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "pub topic");
			bg = get_genlist_item_2text(obj,"Publish Topic", str);
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
get_config_menu_title(void *data, Evas_Object *obj, const char *part)
{
	return strdup("Configuration");
}



void
on_config_update_cb(void *data, Evas_Object *obj, void *event_info)
{
	app_data_s *ad = (app_data_s*)data;
	launch_service_task(RAPROTO_TASK_REQUEST_SETTINGS_UPDATE, ad);
	return;
}


static void
on_config_publish_cb(void *data, Evas_Object *obj, void *event_info)
{
	app_data_s *ad = (app_data_s*)data;
	char *pub_topic;
	bundle_get_str(ad->settings, RAPROTO_SETTING_MQTT_CONFIG_PUB_TOPIC, &pub_topic);
	return create_entry(ad, ELM_INPUT_PANEL_LAYOUT_URL, pub_topic, return_publish_topic);
}


static void
on_config_subscribe_cb(void *data, Evas_Object *obj, void *event_info)
{
	app_data_s *ad = (app_data_s*)data;
	char *sub_topic;
	bundle_get_str(ad->settings, RAPROTO_SETTING_MQTT_CONFIG_SUB_TOPIC, &sub_topic);
	return create_entry(ad, ELM_INPUT_PANEL_LAYOUT_URL, sub_topic, return_subscribe_topic);
}



void
create_config_menu(app_data_s *ad) {

	Evas_Smart_Cb func[NUM_OF_CONFIG_MENU_ITEMS];
	func[CONFIG_UPDATE] = on_config_update_cb;
	func[CONFIG_SUBSCRIBE] = on_config_subscribe_cb;
	func[CONFIG_PUBLISH] = on_config_publish_cb;

	menu_data_s menu;
	menu.title_text = get_config_menu_title;
	menu.item_content = get_config_menu_content;
	menu.num_items = NUM_OF_CONFIG_MENU_ITEMS;
	menu.func = func;

	Evas_Object *frame = create_frame(&(ad->frame_config), &menu, ad);
	elm_naviframe_item_push(ad->nf, NULL, NULL, NULL, frame, "empty");
}
