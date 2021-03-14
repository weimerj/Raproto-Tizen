



#include "main_menu.h"






static void
main_check_changed_item_cb(void *data, Evas_Object *obj, void *event_info){
	service_data_s *service = (service_data_s *)data;
	int err;
	bundle *b = bundle_create();

	if (service->error){
		bundle_add_str(b, RAPROTO_TASK_REQUEST, RAPROTO_TASK_REQUEST_LOG_OFF);
		elm_check_state_set(obj, EINA_FALSE); // UNTESTED: NOT SURE IF THIS IS RIGHT
	} else if (service->logging) {
		bundle_add_str(b, RAPROTO_TASK_REQUEST, RAPROTO_TASK_REQUEST_LOG_OFF);
	} else {
		bundle_add_str(b, RAPROTO_TASK_REQUEST, RAPROTO_TASK_REQUEST_LOG_ON);
	}

	if ((err = event_publish_app_event(RAPROTO_LAUNCH_TASK_EVENT, b)) != EVENT_ERROR_NONE) error_msg(err,__func__, "launch event");
	bundle_free(b);

	return;
}




/*
 * Get content of main menu
 */
static Evas_Object*
get_main_menu_content(void *data, Evas_Object *obj, const char *part)
{
	list_item_data_s *lid = (list_item_data_s *)data;
	Evas_Object *bg = NULL;
	char msg[MAX_STRING_SIZE];
	Evas_Object *check;
	int err;
	char *str;
	char *version;

	//dlog_print(DLOG_INFO, LOG_TAG, "%d : %s", lid->index, part);

	// only using elm.swallow.content -- anything else is an error.
	if (strcmp("elm.swallow.content", part)){
		error_msg(EVAS_LOAD_ERROR_GENERIC, __func__, part);
		return NULL;
	}

	switch (lid->index) {
		case (MAIN_MENU_DEVICE_ID):
			if ((err = bundle_get_str(*(lid->settings), RAPROTO_SETTING_DEVICE_ID, &str)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "device id");
			bg = get_genlist_item_2text(obj,"Device ID",str);
			break;
		case (MAIN_MENU_ONOFF):
			bg = get_genlist_item_2text1check(obj, "ON / OFF", "\0", &check);
			evas_object_smart_callback_add(check, "changed",  main_check_changed_item_cb, lid->service);
			if (lid->service->logging) elm_check_state_set(check, EINA_TRUE);
			else elm_check_state_set(check, EINA_FALSE);
			break;
		case (MAIN_MENU_SETTINGS):
			bg = get_genlist_item_1text(obj,"Settings");
			break;
		case (MAIN_MENU_SYNC):
			if ((err = bundle_get_str(*(lid->settings), RAPROTO_SETTING_DATA_STORED, &str)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "sync status");
			sprintf(msg,"%s", str);
			bg = get_genlist_item_2text(obj,"Sync Now", msg);
			break;
		case (MAIN_MENU_ABOUT):
			if ((err = app_get_version(&version)) != APP_ERROR_NONE) error_msg(err, __func__, "version");
			bg = get_genlist_item_2text(obj,"About", version);
			break;
		case (MAIN_MENU_EXIT):
			bg = get_genlist_item_2text(obj,"Exit", "save and stop");
			break;
		default:
			return NULL;
	}

	get_background_for_state(bg,lid->service);

	return bg;
}



/*
 * Get title of the main view
 */
static char *
get_main_menu_title(void *data, Evas_Object *obj, const char *part)
{

	/* Item data for main UI */
	list_item_data_s *lid = (list_item_data_s*)data;
	int err;
	char *str;
	char msg[MAX_STRING_SIZE];
	if ((err = bundle_get_str(*(lid->settings), RAPROTO_SETTING_NAME, &str)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "name");
		sprintf(msg,"<align=center>Raproto<ps>%s</align>", str);


	return strdup(msg);
}



void
return_subject_id(void *data, Evas_Object *entry, void *event_info){
	app_data_s *ad = (app_data_s*)data;
	int err;

	if ((err = app_control_remove_extra_data(ad->service.control, RAPROTO_SETTING_DEVICE_ID)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "remove extra data");
	if ((err = app_control_add_extra_data(ad->service.control, RAPROTO_SETTING_DEVICE_ID, elm_entry_entry_get(entry))) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "extra data");

	launch_service_task(RAPROTO_TASK_REQUEST_SETTINGS_TRANSMIT, ad);

	elm_naviframe_item_pop(ad->nf);

	return;
}



static void
on_about_cb(void *data, Evas_Object *obj, void *event_info) {
	app_data_s *ad = (app_data_s*)data;
	return create_about_menu(ad);
}


static void
on_settings_cb(void *data, Evas_Object *obj, void *event_info) {
	app_data_s *ad = (app_data_s*)data;
	if (ad->service.logging) return;

	return create_settings_menu(ad);
}

static void
on_sync_cb(void *data, Evas_Object *obj, void *event_info) {
	app_data_s *ad = (app_data_s*)data;
	launch_service_task(RAPROTO_TASK_REQUEST_TRANSMIT, ad);
	return;
}


static void
on_onoff_cb(void *data, Evas_Object *obj, void *event_info) {
	//app_data_s *ad = (app_data_s*)data;
	return;
}


static void
on_subject_id_cb(void *data, Evas_Object *obj, void *event_info) {
	app_data_s *ad = (app_data_s*)data;
	if (ad->service.logging) return;
	char *id;
	bundle_get_str(ad->settings, RAPROTO_SETTING_DEVICE_ID, &id);
	return create_entry(ad, ELM_INPUT_PANEL_LAYOUT_NUMBERONLY, id, return_subject_id);
}


static void
on_stop_cb(void *data, Evas_Object *obj, void *event_info) {
	app_data_s *ad = (app_data_s*)data;

	// THe following was a nice idea, but doesn't seem to work especially well.
	//if (!(ad->service.logging)) launch_service_task(RAPROTO_TASK_REQUEST_STOP, ad);
	//else launch_service_task(RAPROTO_TASK_REQUEST_CLEAR_QUEUE, ad);

	launch_service_task(RAPROTO_TASK_REQUEST_STOP, ad);
	ui_app_exit();
	return;
}



/*
 * Draw Raproto's main menu
 */
void
create_main_menu(app_data_s *ad) {
	Elm_Object_Item *nf_it;

	Evas_Smart_Cb func[NUM_OF_MAIN_MENU_ITEMS];
	func[MAIN_MENU_DEVICE_ID] = on_subject_id_cb;
	func[MAIN_MENU_ONOFF] = on_onoff_cb;
	func[MAIN_MENU_SYNC] = on_sync_cb;
	func[MAIN_MENU_SETTINGS] = on_settings_cb;
	func[MAIN_MENU_EXIT] = on_stop_cb;
	func[MAIN_MENU_ABOUT] = on_about_cb;

	menu_data_s menu;
	menu.title_text = get_main_menu_title;
	menu.item_content = get_main_menu_content;
	menu.num_items = NUM_OF_MAIN_MENU_ITEMS;
	menu.func = func;

	Evas_Object *frame = create_frame(&(ad->frame_main), &menu, ad);
	nf_it = elm_naviframe_item_push(ad->nf, NULL, NULL, NULL, frame, "empty");
	elm_naviframe_item_pop_cb_set(nf_it, win_delete_request_cb, ad->win);
}


void
create_base_gui(app_data_s *ad) {
	// Window
	ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
	elm_win_autodel_set(ad->win, EINA_TRUE);

	if (elm_win_wm_rotation_supported_get(ad->win)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(ad->win, (const int *)(&rots), 4);
	}

	// Conformant
	ad->conform = elm_conformant_add(ad->win);
	elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
	elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);
	evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(ad->win, ad->conform);
	evas_object_show(ad->conform);

	// Eext Circle Surface Creation
	ad->circle_surface = eext_circle_surface_conformant_add(ad->conform);

	// Base Layout
	ad->layout = elm_layout_add(ad->conform);
	evas_object_size_hint_weight_set(ad->layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_layout_theme_set(ad->layout, "layout", "application", "default");
	evas_object_show(ad->layout);
	elm_object_content_set(ad->conform, ad->layout);

	// Naviframe
	ad->nf = elm_naviframe_add(ad->layout);
	elm_object_part_content_set(ad->layout, "elm.swallow.content", ad->nf);
	elm_naviframe_event_enabled_set(ad->nf, EINA_TRUE);
	eext_object_event_callback_add(ad->nf, EEXT_CALLBACK_BACK, eext_naviframe_back_cb, NULL);
	eext_object_event_callback_add(ad->nf, EEXT_CALLBACK_MORE, eext_naviframe_more_cb, NULL);

	// create the main menu
	create_main_menu(ad);

	// Show window after base gui is set up
	evas_object_show(ad->win);
}

