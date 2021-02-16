/*
 * about_menu.c
 *
 *  Created on: Feb 15, 2021
 *      Author: weimer
 */


#include "about_menu.h"





/*
 * Get content of about menu
 */
static Evas_Object*
get_about_menu_content(void *data, Evas_Object *obj, const char *part)
{
	list_item_data_s *lid = (list_item_data_s *)data;
	Evas_Object *bg = NULL;
	char *version;
	int err;

	//dlog_print(DLOG_INFO, LOG_TAG, "%d : %s", lid->index, part);

	// only using elm.swallow.content -- anything else is an error.
	if (strcmp("elm.swallow.content", part)){
		error_msg(EVAS_LOAD_ERROR_GENERIC, __func__, part);
		return NULL;
	}


	switch (lid->index) {
		case (ABOUT_VERSION):
			if ((err = app_get_version(&version)) != APP_ERROR_NONE) error_msg(err, __func__, "version");
			bg = get_genlist_item_2text(obj,"Version", version);
			break;
		case (ABOUT_INFO):
			bg = get_genlist_item_1text(obj,"Info");
			break;
		case (ABOUT_DISCLAIMER):
			bg = get_genlist_item_1text(obj,"Disclaimer");
			break;
		case (ABOUT_HELP):
			bg = get_genlist_item_1text(obj,"Help");
			break;
		default:
			return NULL;
	}

	get_background_for_state(bg,lid->service);

	return bg;
}


/*
 * Get title of the about view
 */
static char *
get_about_menu_title(void *data, Evas_Object *obj, const char *part)
{
	return strdup("About Raproto");
}



static void
on_version_cb(void *data, Evas_Object *obj, void *event_info) {
	app_data_s *ad = (app_data_s*)data;
	ad->service.reset_taps++;
	if (ad->service.reset_taps >= RAPROTO_ERROR_RESET_TAPS) {
		ad->service.reset = true;
		launch_service_task(RAPROTO_TASK_REQUEST_FACTORY_RESET, ad);
		ui_app_exit();
	}
	return;
}

static void
on_info_cb(void *data, Evas_Object *obj, void *event_info) {
	app_data_s *ad = (app_data_s*)data;
	return create_text_display(ad,info_message);
}

static void
on_disclaimer_cb(void *data, Evas_Object *obj, void *event_info) {
	app_data_s *ad = (app_data_s*)data;
	return create_text_display(ad,disclaimer_message);
}

static void
on_help_cb(void *data, Evas_Object *obj, void *event_info) {
	app_data_s *ad = (app_data_s*)data;
	return create_text_display(ad,help_message);
}


void
create_about_menu(app_data_s *ad) {
	Evas_Smart_Cb func[NUM_OF_ABOUT_MENU_ITEMS];
	func[ABOUT_VERSION] = on_version_cb;
	func[ABOUT_INFO] = on_info_cb;
	func[ABOUT_DISCLAIMER] = on_disclaimer_cb;
	func[ABOUT_HELP] = on_help_cb;

	menu_data_s menu;
	menu.title_text = get_about_menu_title;
	menu.item_content = get_about_menu_content;
	menu.num_items = NUM_OF_ABOUT_MENU_ITEMS;
	menu.func = func;

	Evas_Object *frame = create_frame(&(ad->frame_about), &menu, ad);
	elm_naviframe_item_push(ad->nf, NULL, NULL, NULL, frame, "empty");

	ad->service.reset_taps = 0;
}
