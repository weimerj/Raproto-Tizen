#include <raproto.h>


//static void create_base_gui(app_data_s *);
//static void launch_service_task(char *, app_data_s *);

//TODO: Change some errors to warnings (ones we don't want to to error on)
//static void
//warn_msg(int rc, char const *caller_name, char const *id) {
//	dlog_print(DLOG_WARN, LOG_TAG, "%s : %s : [%d, %s]",caller_name, id, rc, get_error_message(rc));
//	return;
//}

//
//static void
//error_msg(int rc, char const *caller_name, char const *id) {
//	dlog_print(DLOG_ERROR, LOG_TAG, "%s : %s : [%d, %s]",caller_name, id, rc, get_error_message(rc));
//	return;
//}
//
//
//static void
//update_theme_frame(frame_data_s *frame, int r, int g, int b){
//	if ((frame->gl == NULL) || (frame->bg == NULL)) return;
//
//	int default_theme[3] = {RAPROTO_COLOR_BACKGROUND_DEFAULT};
//
//	if ((r == default_theme[0]) && (g == default_theme[1]) && (b == default_theme[2])){
//		elm_genlist_realized_items_update(frame->gl);
//		elm_bg_color_set(frame->bg,r,g,b);
//		elm_object_style_set(frame->gl, "default");
//	} else {
//		elm_genlist_realized_items_update(frame->gl);
//		elm_bg_color_set(frame->bg,r,g,b);
//		elm_object_style_set(frame->gl, "transparent");
//	}
//}
//
//
//static void
//update_theme_all(int r, int g, int b, app_data_s *ad){
//	update_theme_frame(&(ad->frame_main), r, g, b);
//	update_theme_frame(&(ad->frame_settings), r, g, b);
//	update_theme_frame(&(ad->frame_mqtt), r, g, b);
//	update_theme_frame(&(ad->frame_config), r, g, b);
//	update_theme_frame(&(ad->frame_about), r, g, b);
//}
//
//
//
//static void
//update_theme(app_data_s *ad){
//
//	static int prev_state = 0;
//	int current_state;
//
//	if (ad->service.error) current_state = -1;
//	else if (ad->service.logging) current_state = 1;
//	else current_state = 0;
//
//	if (current_state == prev_state) return;
//	else prev_state = current_state;
//
//	if (ad->service.error) update_theme_all(RAPROTO_COLOR_BACKGROUND_ERROR, ad);
//	else if (ad->service.logging) update_theme_all(RAPROTO_COLOR_BACKGROUND_LOGGING, ad);
//	else update_theme_all(RAPROTO_COLOR_BACKGROUND_DEFAULT, ad);
//
//}
//
//
//
//static void
//update_frame(Evas_Object *gl, int num_entries) {
//
//	Elm_Object_Item *it;
//
//	for (int j = 0; j <= num_entries; j++) {
//		it = elm_genlist_nth_item_get(gl, j);
//		elm_genlist_item_update(it);
//	}
//
//}

//
//static void
//update_settings_user(char *key, app_data_s *ad){
//	int err;
//	char *str;
//	if ((err = bundle_get_str(ad->settings, key, &str)) != BUNDLE_ERROR_NONE) return error_msg(err, __func__, "get str");
//	if (strlen(str) > 0) {
//		if ((err = app_control_add_extra_data(ad->service.control, key, str)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "add data");
//	}
//}

//
//static void
//update_settings(const char *event_name, bundle *settings, void *data){
//	app_data_s *ad = (app_data_s*)data;
//	int err;
//	int *error;
//	int *logging;
//	size_t b_size;
//	bool init_flag;
//
//	if (ad->settings == NULL) {
//
//		init_flag = true;
//		ad->settings = bundle_dup(settings);
//		//dlog_print(DLOG_INFO, LOG_TAG, "starting GUI");
//		create_base_gui(ad); // do this here once settings are created
//
//		update_settings_user(RAPROTO_SETTING_DEVICE_ID, ad);
//		update_settings_user(RAPROTO_SETTING_MQTT_BROKER, ad);
//		update_settings_user(RAPROTO_SETTING_MQTT_USERNAME, ad);
//		update_settings_user(RAPROTO_SETTING_MQTT_PASSWORD, ad);
//		update_settings_user(RAPROTO_SETTING_MQTT_CONFIG_PUB_TOPIC, ad);
//		update_settings_user(RAPROTO_SETTING_MQTT_CONFIG_SUB_TOPIC, ad);
//	} else {
//		init_flag = false;
//		if ((err = bundle_free(ad->settings)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "bundle free");
//		ad->settings = bundle_dup(settings);
//	}
//
//	if ((err = bundle_get_byte(ad->settings, RAPROTO_SETTING_ERROR, (void**)&error, &b_size)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get error byte");
//	if ((err = bundle_get_byte(ad->settings, RAPROTO_SETTING_LOGGING, (void**)&logging, &b_size)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get logging byte");
//
//	switch(*error) {
//		case (RAPROTO_SETTING_ERROR_TRUE):
//			ad->service.error = true;
//			break;
//		case (RAPROTO_SETTING_ERROR_FALSE):
//			ad->service.error = false;
//			break;
//		default:
//			error_msg(BUNDLE_ERROR_INVALID_PARAMETER, __func__, "unknown error byte");
//	}
//
//	switch(*logging){
//		case (RAPROTO_SETTING_LOGGING_TRUE):
//			ad->service.logging = true;
//			break;
//		case (RAPROTO_SETTING_LOGGING_FALSE):
//			ad->service.logging = false;
//			break;
//		default:
//			error_msg(BUNDLE_ERROR_INVALID_PARAMETER, __func__, "unknown logging byte");
//	}
//
//	if (ad->frame_main.gl != NULL) update_frame(ad->frame_main.gl, NUM_OF_MAIN_MENU_ITEMS);
//	if (ad->frame_settings.gl != NULL) update_frame(ad->frame_settings.gl, NUM_OF_SETTINGS_MENU_ITEMS);
//	if (ad->frame_about.gl != NULL) update_frame(ad->frame_about.gl, NUM_OF_ABOUT_MENU_ITEMS);
//	if (ad->frame_mqtt.gl != NULL) update_frame(ad->frame_mqtt.gl, NUM_OF_MQTT_MENU_ITEMS);
//	if (ad->frame_config.gl != NULL) update_frame(ad->frame_config.gl, NUM_OF_CONFIG_MENU_ITEMS);
//	update_theme(ad);
//
//	dlog_print(DLOG_INFO, LOG_TAG, "settings updated");
//}


//
//
///*
// * Release list item data structure
// */
//static void
//gl_del_cb(void *data, Evas_Object *obj)
//{
//	list_item_data_s *lid = (list_item_data_s*)data;
//	lid->index = 0;
//	lid->settings = NULL;
//	lid->service = NULL;
//	free(lid);
//}


//
//
//Eina_Bool
//launch_heart_beat(void *data){
//	app_data_s *ad = (app_data_s *)data;
//	int err;
//	bool running;
//	time_t cur_time = time(NULL);
//
//	//dlog_print(DLOG_INFO, LOG_TAG, "heart beat");
//	if ((err = app_manager_is_running(RAPROTO_SERVICE_APP_ID, &running)) != APP_MANAGER_ERROR_NONE) error_msg(err, __func__, "app running");
//
//	update_theme(ad);
//
//	if (cur_time - ad->service.t > 2*RAPROTO_HEART_BEAT){
//		ad->service.error = true;
//	}
//
//	if (ad->service.reboot) {
//		ad->service.t = cur_time;
//	} else if (!(ad->service.error)) {
//		if (running) {
//			if (ad->settings == NULL) {
//				launch_service_task(RAPROTO_TASK_REQUEST_SETTINGS_TRANSMIT, ad);
//				ad->service.t = cur_time;
//			} else if (ad->service.logging) {
//				launch_service_task(RAPROTO_TASK_REQUEST_LOG_CHECK, ad);
//			} else {
//				ad->service.t = cur_time;
//			}
//		} else {
//			ad->service.error = true;
//			launch_service_task(RAPROTO_TASK_REQUEST_ERROR, ad);
//		}
//	}
//
//	return ECORE_CALLBACK_RENEW;
//}
//
//
//
//static void
//launch_heart_beat_cb(app_control_h request, app_control_h reply, app_control_result_e result, void *data) {
//	app_data_s *ad = (app_data_s*)data;
//	ad->service.t = time(NULL);
//	return;
//}
//
//
//
//static void
//launch_service_start_cb(app_control_h request, app_control_h reply, app_control_result_e result, void *data)
//{
//	app_data_s *ad = (app_data_s*)data;
//	ad->service.t = time(NULL);
//	if (!(ad->service.reboot)) {
//		ecore_init();
//		ecore_timer_add(RAPROTO_HEART_BEAT, launch_heart_beat, ad);
//		ecore_main_loop_begin();
//	} else {
//		ad->service.reboot = false;
//	}
//
//}
//
//
//static void
//launch_service_start(app_data_s *ad) {
//	int err;
//
//	if (ad->service.control == NULL) {
//		if ((err = app_control_create(&(ad->service.control))) != APP_CONTROL_ERROR_NONE) return error_msg(err,__func__, "control create");
//		if ((err = app_control_set_operation(ad->service.control, APP_CONTROL_OPERATION_DEFAULT)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "set operation");
//		if ((err = app_control_set_app_id(ad->service.control, RAPROTO_SERVICE_APP_ID)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "setup app id");
//	}
//	if ((err = app_control_add_extra_data(ad->service.control, RAPROTO_TASK_REQUEST, RAPROTO_TASK_REQUEST_START)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "extra data");
//	if ((err = app_control_send_launch_request(ad->service.control, launch_service_start_cb, ad)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "send launch");
//
//}
//
//
//
//static void
//launch_service_task_wait_for_response(app_control_h *control, app_control_reply_cb callback, void *user_data) {
//	int err;
//	while ((err = app_control_send_launch_request(*control, callback, user_data)) != APP_CONTROL_ERROR_NONE) {
//		if (err == APP_CONTROL_ERROR_LAUNCH_REJECTED){
//			usleep(RAPROTO_MUTEX_SLEEP);
//		} else {
//			return error_msg(err, __func__, "send launch");
//		}
//	}
//}
//
//
//static void
//launch_service_task_wait_for_terminate(app_data_s *ad) {
//	int err;
//	bool running;
//	ad->service.reboot = true;
//
//	launch_service_task_wait_for_response(&(ad->service.control), NULL, NULL);
//
//	if ((err = app_manager_is_running(RAPROTO_SERVICE_APP_ID, &running)) != APP_MANAGER_ERROR_NONE) error_msg(err, __func__, "app running");
//	while (running) {
//		usleep(RAPROTO_MUTEX_SLEEP);
//		if ((err = app_manager_is_running(RAPROTO_SERVICE_APP_ID, &running)) != APP_MANAGER_ERROR_NONE) error_msg(err, __func__, "app running");
//	}
//
//	launch_service_start(ad);
//}
//
//static void
//launch_service_task(char *task, app_data_s *ad){
//	int err;
//	bool running;
//
//	if (ad->service.control == NULL) error_msg(APP_CONTROL_ERROR_NONE, __func__, "no control");
//
//	if ((err = app_manager_is_running(RAPROTO_SERVICE_APP_ID, &running)) != APP_MANAGER_ERROR_NONE) error_msg(err, __func__, "app running");
//
//	if ((!(ad->service.error)) || (!strcmp(task,RAPROTO_TASK_REQUEST_FACTORY_RESET))) {
//		if (running) {
//			if ((err = app_control_remove_extra_data(ad->service.control, RAPROTO_TASK_REQUEST)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "remove extra data");
//			if ((err = app_control_add_extra_data(ad->service.control, RAPROTO_TASK_REQUEST, task)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "add extra data");
//
//			if (!strcmp(task,RAPROTO_TASK_REQUEST_LOG_CHECK)) {
//				launch_service_task_wait_for_response(&(ad->service.control), launch_heart_beat_cb, (void*)ad);
//			} else if (!strcmp(task, RAPROTO_TASK_REQUEST_LOG_OFF)) {
//				launch_service_task_wait_for_terminate(ad);
//			}	else {
//				launch_service_task_wait_for_response(&(ad->service.control), NULL, NULL);
//			}
//		} else {
//			ad->service.error = true;
//		}
//	}
//}
//
//
//static void
//launch_service_task_event(const char *event_name, bundle *task_b, void *data){
//	app_data_s *ad = (app_data_s*)data;
//	int err;
//	char *task;
//
//	if ((err = bundle_get_str(task_b, RAPROTO_TASK_REQUEST, &task)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get str");
//	launch_service_task(task, ad);
//
//}
//
//
//





//
//static void
//main_check_changed_item_cb(void *data, Evas_Object *obj, void *event_info){
//	service_data_s *service = (service_data_s *)data;
//	int err;
//	bundle *b = bundle_create();
//
//	if (service->logging) {
//		bundle_add_str(b, RAPROTO_TASK_REQUEST, RAPROTO_TASK_REQUEST_LOG_OFF);
//	} else {
//		bundle_add_str(b, RAPROTO_TASK_REQUEST, RAPROTO_TASK_REQUEST_LOG_ON);
//	}
//
//	if ((err = event_publish_app_event(RAPROTO_LAUNCH_TASK_EVENT, b)) != EVENT_ERROR_NONE) error_msg(err,__func__, "launch event");
//	bundle_free(b);
//
//	return;
//}

//
//
//Evas_Object*
//get_genlist_item_2text1check(Evas_Object *obj, const char *text, const char *subtext, Evas_Object **check){
//
//	char str[MAX_STRING_SIZE + MAX_MARKUP_SIZE] = {0,};
//
//	Evas_Object *bg = elm_bg_add(obj);
//	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
//	evas_object_size_hint_align_set(bg, EVAS_HINT_FILL, EVAS_HINT_FILL);
//	evas_object_size_hint_min_set(bg, 100, 100);
//	evas_object_show(bg);
//
//	Evas_Object *hbox = elm_box_add(bg);
//	evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
//	evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, EVAS_HINT_FILL);
//	elm_box_horizontal_set(hbox, EINA_TRUE);
//	elm_object_content_set(bg, hbox);
//	evas_object_show(hbox);
//
//
//	Evas_Object *box = elm_box_add(hbox);
//	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
//	evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
//	evas_object_show(box);
//	elm_box_pack_end(hbox, box);
//
//	Evas_Object *label = elm_label_add(box);
//	snprintf(str, MAX_STRING_SIZE+MAX_MARKUP_SIZE, "%s%s", font_markup_main, text);
//	elm_object_text_set(label, str);
//	evas_object_show(label);
//	elm_box_pack_end(box, label);
//
//	Evas_Object *sublabel = elm_label_add(box);
//	snprintf(str, MAX_STRING_SIZE+MAX_MARKUP_SIZE, "%s%s", font_markup_sub, subtext);
//	elm_object_text_set(sublabel, str);
//	evas_object_show(sublabel);
//	elm_box_pack_end(box, sublabel);
//
//	//Evas_Object *check;
//	*check = elm_check_add(hbox);
//	evas_object_show(*check);
//	elm_box_pack_end(hbox,*check);
//
//	Evas_Object *spacer = evas_object_rectangle_add(hbox);
//	evas_object_size_hint_min_set(spacer, 50, 0);
//	evas_object_color_set(spacer, 0, 0, 0, 0);
//	elm_box_pack_end(hbox, spacer);
//
//	return bg;
//}
//
//
//Evas_Object*
//get_genlist_item_1text(Evas_Object *obj, const char *text){
//
//	char str[MAX_STRING_SIZE + MAX_MARKUP_SIZE] = {0,};
//
//	Evas_Object *bg = elm_bg_add(obj);
//	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
//	evas_object_size_hint_align_set(bg, EVAS_HINT_FILL, EVAS_HINT_FILL);
//	evas_object_size_hint_min_set(bg, 100, 100);
//	//elm_bg_color_set(bg, RAPROTO_BACKGROUND_ERROR);
//	evas_object_show(bg);
//
//	Evas_Object *box = elm_box_add(bg);
//	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
//	evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
//	evas_object_show(box);
//	elm_object_content_set(bg, box);
//
//	Evas_Object *label = elm_label_add(box);
//	snprintf(str, MAX_STRING_SIZE+MAX_MARKUP_SIZE, "%s%s", font_markup_main, text);
//	elm_object_text_set(label, str);
//	evas_object_show(label);
//	elm_box_pack_end(box, label);
//
//	return bg;
//}
//
//
//
//Evas_Object*
//get_genlist_item_2text(Evas_Object *obj, const char *text, const char *subtext){
//
//	char str[MAX_STRING_SIZE + MAX_MARKUP_SIZE] = {0,};
//
//	Evas_Object *bg = elm_bg_add(obj);
//	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
//	evas_object_size_hint_align_set(bg, EVAS_HINT_FILL, EVAS_HINT_FILL);
//	evas_object_size_hint_min_set(bg, 100, 100);
//	//elm_bg_color_set(bg, RAPROTO_BACKGROUND_ERROR);
//	evas_object_show(bg);
//
//	Evas_Object *box = elm_box_add(bg);
//	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
//	evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
//	evas_object_show(box);
//	elm_object_content_set(bg, box);
//
//	Evas_Object *label = elm_label_add(box);
//	snprintf(str, MAX_STRING_SIZE+MAX_MARKUP_SIZE, "%s%s", font_markup_main, text);
//	elm_object_text_set(label, str);
//	evas_object_show(label);
//	elm_box_pack_end(box, label);
//
//	Evas_Object *sublabel = elm_label_add(box);
//	snprintf(str, MAX_STRING_SIZE+MAX_MARKUP_SIZE, "%s%s", font_markup_sub, subtext);
//	elm_object_text_set(sublabel, str);
//	evas_object_show(sublabel);
//	elm_box_pack_end(box, sublabel);
//
//	return bg;
//}
//
//
//static void
//get_background_for_state(Evas_Object *bg, service_data_s *service){
//	if (service->error) elm_bg_color_set(bg, RAPROTO_COLOR_GENLIST_ERROR);
//	else if (service->logging) elm_bg_color_set(bg, RAPROTO_COLOR_GENLIST_LOGGING);
//	else elm_bg_color_set(bg, RAPROTO_COLOR_GENLIST_DEFAULT);
//}
//


//
//
///*
// * Get content of config menu
// */
//static Evas_Object*
//get_config_menu_content(void *data, Evas_Object *obj, const char *part)
//{
//	list_item_data_s *lid = (list_item_data_s *)data;
//	Evas_Object *bg = NULL;
//	int err;
//	char *str;
//
//	//dlog_print(DLOG_INFO, LOG_TAG, "%d : %s", lid->index, part);
//
//	// only using elm.swallow.content -- anything else is an error.
//	if (strcmp("elm.swallow.content", part)){
//		error_msg(EVAS_LOAD_ERROR_GENERIC, __func__, part);
//		return NULL;
//	}
//
//	switch (lid->index) {
//		case (CONFIG_UPDATE):
//			if ((err = bundle_get_str(lid->settings, RAPROTO_SETTING_CONFIG_STATUS, &str)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "config status");
//			bg = get_genlist_item_2text(obj,"Update", str);
//			break;
//		case (CONFIG_SUBSCRIBE):
//			if ((err = bundle_get_str(lid->settings, RAPROTO_SETTING_MQTT_CONFIG_SUB_TOPIC, &str)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "sub topic");
//			bg = get_genlist_item_2text(obj,"Subscribe Topic", str);
//			break;
//		case (CONFIG_PUBLISH):
//			if ((err = bundle_get_str(lid->settings, RAPROTO_SETTING_MQTT_CONFIG_PUB_TOPIC, &str)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "pub topic");
//			bg = get_genlist_item_2text(obj,"Publish Topic", str);
//			break;
//		default:
//			return NULL;
//	}
//
//	get_background_for_state(bg,lid->service);
//
//	return bg;
//}
//
//
//
///*
// * Get title of the mqtt view
// */
//static char *
//get_config_menu_title(void *data, Evas_Object *obj, const char *part)
//{
//	return strdup("Configuration");
//}



//
//
///*
// * Get content of mqtt menu
// */
//static Evas_Object*
//get_mqtt_menu_content(void *data, Evas_Object *obj, const char *part)
//{
//	list_item_data_s *lid = (list_item_data_s *)data;
//	Evas_Object *bg = NULL;
//	int err;
//	char *str;
//
//	//dlog_print(DLOG_INFO, LOG_TAG, "%d : %s", lid->index, part);
//
//	// only using elm.swallow.content -- anything else is an error.
//	if (strcmp("elm.swallow.content", part)){
//		error_msg(EVAS_LOAD_ERROR_GENERIC, __func__, part);
//		return NULL;
//	}
//
//	switch (lid->index) {
//		case (MQTT_BROKER):
//			if ((err = bundle_get_str(lid->settings, RAPROTO_SETTING_MQTT_BROKER, &str)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "broker");
//			bg = get_genlist_item_2text(obj,"Broker Address", str);
//			break;
//		case (MQTT_USERNAME):
//			if ((err = bundle_get_str(lid->settings, RAPROTO_SETTING_MQTT_USERNAME, &str)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "username");
//			bg = get_genlist_item_2text(obj,"Username", str);
//			break;
//		case (MQTT_PASSWORD):
//			bg = get_genlist_item_2text(obj,"Password", NULL);
//			break;
//		default:
//			return NULL;
//	}
//
//	get_background_for_state(bg,lid->service);
//
//	return bg;
//}
//
//
//
///*
// * Get title of the mqtt view
// */
//static char *
//get_mqtt_menu_title(void *data, Evas_Object *obj, const char *part)
//{
//	return strdup("MQTT Settings");
//}


//
//
//
///*
// * Get content of about menu
// */
//static Evas_Object*
//get_about_menu_content(void *data, Evas_Object *obj, const char *part)
//{
//	list_item_data_s *lid = (list_item_data_s *)data;
//	Evas_Object *bg = NULL;
//	char *version;
//	int err;
//
//	//dlog_print(DLOG_INFO, LOG_TAG, "%d : %s", lid->index, part);
//
//	// only using elm.swallow.content -- anything else is an error.
//	if (strcmp("elm.swallow.content", part)){
//		error_msg(EVAS_LOAD_ERROR_GENERIC, __func__, part);
//		return NULL;
//	}
//
//
//	switch (lid->index) {
//		case (ABOUT_VERSION):
//			if ((err = app_get_version(&version)) != APP_ERROR_NONE) error_msg(err, __func__, "version");
//			bg = get_genlist_item_2text(obj,"Version", version);
//			break;
//		case (ABOUT_INFO):
//			bg = get_genlist_item_1text(obj,"Info");
//			break;
//		case (ABOUT_DISCLAIMER):
//			bg = get_genlist_item_1text(obj,"Disclaimer");
//			break;
//		case (ABOUT_HELP):
//			bg = get_genlist_item_1text(obj,"Help");
//			break;
//		default:
//			return NULL;
//	}
//
//	get_background_for_state(bg,lid->service);
//
//	return bg;
//}
//
//
///*
// * Get title of the about view
// */
//static char *
//get_about_menu_title(void *data, Evas_Object *obj, const char *part)
//{
//	return strdup("About Raproto");
//}


//
//
///*
// * Get content of settings menu
// */
//static Evas_Object*
//get_settings_menu_content(void *data, Evas_Object *obj, const char *part)
//{
//	list_item_data_s *lid = (list_item_data_s *)data;
//	Evas_Object *bg = NULL;
//	int err;
//	char *str;
//
//	//dlog_print(DLOG_INFO, LOG_TAG, "%d : %s", lid->index, part);
//
//	// only using elm.swallow.content -- anything else is an error.
//	if (strcmp("elm.swallow.content", part)){
//		error_msg(EVAS_LOAD_ERROR_GENERIC, __func__, part);
//		return NULL;
//	}
//
//
//	switch (lid->index) {
//		case (SETTINGS_CONFIG):
//			if ((err = bundle_get_str(lid->settings, RAPROTO_SETTING_LAST_UPDATE, &str)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "config status");
//			bg = get_genlist_item_2text(obj,"Configuration",str);
//			break;
//		case (SETTINGS_MQTT):
//			if ((err = bundle_get_str(lid->settings, RAPROTO_SETTING_MQTT_BROKER, &str)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "broker");
//			bg = get_genlist_item_2text(obj,"MQTT", str);
//			break;
//		default:
//			return NULL;
//	}
//
//	get_background_for_state(bg,lid->service);
//
//	return bg;
//}
//
//
///*
// * Get title of the settings view
// */
//static char *
//get_settings_menu_title(void *data, Evas_Object *obj, const char *part)
//{
//	return strdup("Settings");
//}


//
///*
// * Get content of main menu
// */
//static Evas_Object*
//get_main_menu_content(void *data, Evas_Object *obj, const char *part)
//{
//	list_item_data_s *lid = (list_item_data_s *)data;
//	Evas_Object *bg = NULL;
//	char msg[MAX_STRING_SIZE];
//	Evas_Object *check;
//	int err;
//	char *str;
//	char *version;
//
//	//dlog_print(DLOG_INFO, LOG_TAG, "%d : %s", lid->index, part);
//
//	// only using elm.swallow.content -- anything else is an error.
//	if (strcmp("elm.swallow.content", part)){
//		error_msg(EVAS_LOAD_ERROR_GENERIC, __func__, part);
//		return NULL;
//	}
//
//	switch (lid->index) {
//		case (MAIN_MENU_DEVICE_ID):
//			if ((err = bundle_get_str(lid->settings, RAPROTO_SETTING_DEVICE_ID, &str)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "device id");
//			bg = get_genlist_item_2text(obj,"Device ID",str);
//			break;
//		case (MAIN_MENU_ONOFF):
//			bg = get_genlist_item_2text1check(obj, "ON / OFF", "\0", &check);
//			evas_object_smart_callback_add(check, "changed",  main_check_changed_item_cb, lid->service);
//			if (lid->service->logging) elm_check_state_set(check, EINA_TRUE);
//			else elm_check_state_set(check, EINA_FALSE);
//			break;
//		case (MAIN_MENU_SETTINGS):
//			bg = get_genlist_item_1text(obj,"Settings");
//			break;
//		case (MAIN_MENU_SYNC):
//			if ((err = bundle_get_str(lid->settings, RAPROTO_SETTING_LAST_SYNC, &str)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "sync status");
//			sprintf(msg,"Synced: %s", str);
//			bg = get_genlist_item_2text(obj,"Sync Now", msg);
//			break;
//		case (MAIN_MENU_ABOUT):
//			if ((err = app_get_version(&version)) != APP_ERROR_NONE) error_msg(err, __func__, "version");
//			bg = get_genlist_item_2text(obj,"About", version);
//			break;
//		default:
//			return NULL;
//	}
//
//	get_background_for_state(bg,lid->service);
//
//	return bg;
//}


//
///*
// * Get title of the main view
// */
//static char *
//get_main_menu_title(void *data, Evas_Object *obj, const char *part)
//{
//
//	/* Item data for main UI */
//	list_item_data_s *lid = (list_item_data_s*)data;
//	int err;
//	char *str;
//	char msg[MAX_STRING_SIZE];
//	if ((err = bundle_get_str(lid->settings, RAPROTO_SETTING_NAME, &str)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "name");
//		sprintf(msg,"<align=center>Raproto<ps>%s</align>", str);
//
//
//	return strdup(msg);
//}
//


//
///*
// * Window delete request callback
// */
//static Eina_Bool
//win_delete_request_cb(void *data, Elm_Object_Item *it)
//{
//	/* Terminate the Sync Adapter sample */
//	ui_app_exit();
//	return EINA_TRUE;
//}

//
//static void
//return_publish_topic(void *data, Evas_Object *entry, void *event_info){
//	app_data_s *ad = (app_data_s*)data;
//	int err;
//
//	if ((err = app_control_remove_extra_data(ad->service.control, RAPROTO_SETTING_MQTT_CONFIG_PUB_TOPIC)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "remove extra data");
//	if ((err = app_control_add_extra_data(ad->service.control, RAPROTO_SETTING_MQTT_CONFIG_PUB_TOPIC, elm_entry_entry_get(entry))) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "extra data");
//
//	launch_service_task(RAPROTO_TASK_REQUEST_SETTINGS_TRANSMIT, ad);
//
//	elm_naviframe_item_pop(ad->nf);
//
//	return;
//}
//
//
//static void
//return_subscribe_topic(void *data, Evas_Object *entry, void *event_info){
//	app_data_s *ad = (app_data_s*)data;
//	int err;
//
//	if ((err = app_control_remove_extra_data(ad->service.control, RAPROTO_SETTING_MQTT_CONFIG_SUB_TOPIC)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "remove extra data");
//	if ((err = app_control_add_extra_data(ad->service.control, RAPROTO_SETTING_MQTT_CONFIG_SUB_TOPIC, elm_entry_entry_get(entry))) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "extra data");
//
//	launch_service_task(RAPROTO_TASK_REQUEST_SETTINGS_TRANSMIT, ad);
//
//	elm_naviframe_item_pop(ad->nf);
//
//	return;
//}
//
//
//static void
//return_password(void *data, Evas_Object *entry, void *event_info){
//	app_data_s *ad = (app_data_s*)data;
//	int err;
//
//	if ((err = app_control_remove_extra_data(ad->service.control, RAPROTO_SETTING_MQTT_PASSWORD)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "remove extra data");
//	if ((err = app_control_add_extra_data(ad->service.control, RAPROTO_SETTING_MQTT_PASSWORD, elm_entry_entry_get(entry))) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "extra data");
//
//	launch_service_task(RAPROTO_TASK_REQUEST_SETTINGS_TRANSMIT, ad);
//
//	elm_naviframe_item_pop(ad->nf);
//
//	return;
//}
//
//
//static void
//return_username(void *data, Evas_Object *entry, void *event_info){
//	app_data_s *ad = (app_data_s*)data;
//	int err;
//
//	if ((err = app_control_remove_extra_data(ad->service.control, RAPROTO_SETTING_MQTT_USERNAME)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "remove extra data");
//	if ((err = app_control_add_extra_data(ad->service.control, RAPROTO_SETTING_MQTT_USERNAME, elm_entry_entry_get(entry))) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "extra data");
//
//	launch_service_task(RAPROTO_TASK_REQUEST_SETTINGS_TRANSMIT, ad);
//
//	elm_naviframe_item_pop(ad->nf);
//
//	return;
//}

//
//static void
//return_broker(void *data, Evas_Object *entry, void *event_info){
//	app_data_s *ad = (app_data_s*)data;
//	int err;
//
//	if ((err = app_control_remove_extra_data(ad->service.control, RAPROTO_SETTING_MQTT_BROKER)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "remove extra data");
//	if ((err = app_control_add_extra_data(ad->service.control, RAPROTO_SETTING_MQTT_BROKER, elm_entry_entry_get(entry))) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "extra data");
//
//	launch_service_task(RAPROTO_TASK_REQUEST_SETTINGS_TRANSMIT, ad);
//
//
//	elm_naviframe_item_pop(ad->nf);
//
//	return;
//}


//static void
//return_subject_id(void *data, Evas_Object *entry, void *event_info){
//	app_data_s *ad = (app_data_s*)data;
//	int err;
//
//	if ((err = app_control_remove_extra_data(ad->service.control, RAPROTO_SETTING_DEVICE_ID)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "remove extra data");
//	if ((err = app_control_add_extra_data(ad->service.control, RAPROTO_SETTING_DEVICE_ID, elm_entry_entry_get(entry))) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "extra data");
//
//	launch_service_task(RAPROTO_TASK_REQUEST_SETTINGS_TRANSMIT, ad);
//
//	elm_naviframe_item_pop(ad->nf);
//
//	return;
//}


//
//static void
//create_text_display(app_data_s *ad, const char *msg)
//{
//
//	Evas_Object *nf = ad->nf, *label, *box, *scroller, *circle_scroller;
//	Elm_Object_Item *nf_it;
//
//	// Create scroller
//    scroller = elm_scroller_add(nf);
//    circle_scroller = eext_circle_object_scroller_add(scroller, ad->circle_surface);
//	eext_circle_object_scroller_policy_set(circle_scroller, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);
//	eext_rotary_object_event_activated_set(circle_scroller, EINA_TRUE);
//    evas_object_size_hint_weight_set(scroller, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
//    evas_object_size_hint_align_set(scroller, EVAS_HINT_FILL,  EVAS_HINT_FILL);
//    evas_object_show(scroller);
//
//	// Create a Box
//	box = elm_box_add(scroller);
//	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
//	evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
//	elm_object_content_set(scroller,box);
//	evas_object_show(box);
//
//    // Add label
//	label = elm_label_add(box);
//    evas_object_size_hint_weight_set(label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
//    evas_object_size_hint_align_set(label, EVAS_HINT_FILL, EVAS_HINT_FILL);
//    evas_object_size_hint_padding_set(label, 50, 50, 50, 240);
//	elm_label_line_wrap_set(label, ELM_WRAP_MIXED);
//    elm_label_wrap_width_set(label,240);
//	elm_object_text_set(label, msg);
//    elm_box_pack_end(box, label);
//    evas_object_show(label);
//
//	nf_it = elm_naviframe_item_push(nf, "Raproto", NULL, NULL, scroller, "empty");
//
//}
//
//
//
//static void
//create_entry(app_data_s *ad, Elm_Input_Panel_Layout layout, char *initial_entry, Evas_Smart_Cb callback){
//
//	//TODO: fix needing to click back twice to exit from the entry widget (no good solution yet)
//
//    // Create a Box
//    Evas_Object *box;
//    box = elm_box_add(ad->nf);
//    evas_object_show(box);
//
//    // Create a keyboard (entry) and add to box
//    Evas_Object *entry;
//    entry = elm_entry_add(box);
//
//	elm_entry_entry_set(entry, initial_entry); // set this to the current ID
//    elm_entry_cursor_pos_set(entry,strlen(initial_entry));
//    elm_entry_single_line_set(entry, EINA_TRUE);
//    elm_entry_scrollable_set(entry, EINA_TRUE);
//    elm_entry_input_panel_return_key_disabled_set(entry, EINA_FALSE);
//    elm_entry_input_panel_layout_set(entry, layout);
//    elm_entry_input_panel_show(entry);
//
//    evas_object_size_hint_weight_set(entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
//    evas_object_size_hint_align_set(entry, EVAS_HINT_FILL,  EVAS_HINT_FILL); // 0.5
//    evas_object_size_hint_padding_set(entry, 50, 50, 30, 240);
//    evas_object_show(entry);
//
//    elm_box_pack_end(box, entry);
//
//    evas_object_smart_callback_add(entry, "activated", callback, ad);
//
//	elm_naviframe_item_push(ad->nf, "Enter ID", NULL, NULL, box, "empty");
//
//}
//
//
//
//
//Evas_Object*
//create_frame(frame_data_s *frame, menu_data_s *md, app_data_s *ad){
//	//create_genlist(Evas_Object **genlist, menu_data_s *md, app_data_s *ad){
//
//	Evas_Object *circle_genlist;
//	list_item_data_s *lid;
//
//	/* Elm_Genlist_Item_Class for title, on/off, list, padding */
//	Elm_Genlist_Item_Class *ttc = elm_genlist_item_class_new();
//	Elm_Genlist_Item_Class *itc = elm_genlist_item_class_new();
//	Elm_Genlist_Item_Class *ptc = elm_genlist_item_class_new();
//
//	frame->bg = elm_bg_add(ad->nf);
//
//    frame->gl = elm_genlist_add(frame->bg);
//	elm_genlist_mode_set(frame->gl, ELM_LIST_SCROLL);
//
//	// pick the right background for the service state
//	if (ad->service.error) {
//		elm_bg_color_set(frame->bg,RAPROTO_COLOR_BACKGROUND_ERROR);
//		elm_object_style_set(frame->gl, "transparent");
//	} else if (ad->service.logging) {
//		elm_bg_color_set(frame->bg,RAPROTO_COLOR_BACKGROUND_LOGGING);
//		elm_object_style_set(frame->gl, "transparent");
//	} else {
//		elm_bg_color_set(frame->bg,RAPROTO_COLOR_BACKGROUND_DEFAULT);
//		elm_object_style_set(frame->gl, "default");
//	}
//
//	/* genlist for circular view */
//	circle_genlist = eext_circle_object_genlist_add(frame->gl, ad->circle_surface);
//	//eext_circle_object_color_set(circle_genlist,255,0,0,255); \\TODO: Pick a Color Scheme for title and scroller
//	eext_circle_object_genlist_scroller_policy_set(circle_genlist, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);
//	eext_rotary_object_event_activated_set(circle_genlist, EINA_TRUE);
//
//
//	ttc->item_style = "title";
//	ttc->func.text_get = md->title_text;
//	ttc->func.del = gl_del_cb;
//
//	itc->item_style = "full";
//	itc->func.content_get = md->item_content;
//	itc->func.del = gl_del_cb;
//
//	ptc->item_style = "padding";
//
//	/* Append genlist item for title */
//	lid = calloc(1,sizeof(list_item_data_s));
//	lid->index = -1;
//	lid->settings = ad->settings;
//	lid->service = &(ad->service);
//	elm_genlist_item_append(frame->gl, ttc, lid, NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);
//
//	/* This logic is for appending genlist items */
//	for (int idx = 0; idx < md->num_items; idx++) {
//
//		lid = calloc(1,sizeof(list_item_data_s));
//		lid->index = idx;
//		lid->settings = ad->settings;
//		lid->service = &(ad->service);
//
//		elm_genlist_item_append(frame->gl, itc, lid, NULL, ELM_GENLIST_ITEM_NONE, md->func[idx], ad);
//	}
//
//	/* Append 2 genlist items for padding */
//	elm_genlist_item_append(frame->gl, ptc, NULL, NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);
//	elm_genlist_item_append(frame->gl, ptc, NULL, NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);
//
//	elm_genlist_item_class_free(ttc);
//	elm_genlist_item_class_free(itc);
//	elm_genlist_item_class_free(ptc);
//
//	evas_object_show(frame->gl);
//	elm_object_content_set(frame->bg, frame->gl);
//
//	return frame->bg;
//}
//

//
//
//static void
//on_config_update_cb(void *data, Evas_Object *obj, void *event_info)
//{
//	app_data_s *ad = (app_data_s*)data;
//	launch_service_task(RAPROTO_TASK_REQUEST_SETTINGS_UPDATE, ad);
//	return;
//}
//
//
//static void
//on_config_publish_cb(void *data, Evas_Object *obj, void *event_info)
//{
//	app_data_s *ad = (app_data_s*)data;
//	char *pub_topic;
//	bundle_get_str(ad->settings, RAPROTO_SETTING_MQTT_CONFIG_PUB_TOPIC, &pub_topic);
//	return create_entry(ad, ELM_INPUT_PANEL_LAYOUT_URL, pub_topic, return_publish_topic);
//}
//
//
//static void
//on_config_subscribe_cb(void *data, Evas_Object *obj, void *event_info)
//{
//	app_data_s *ad = (app_data_s*)data;
//	char *sub_topic;
//	bundle_get_str(ad->settings, RAPROTO_SETTING_MQTT_CONFIG_SUB_TOPIC, &sub_topic);
//	return create_entry(ad, ELM_INPUT_PANEL_LAYOUT_URL, sub_topic, return_subscribe_topic);
//}
//
//
//
//static void
//create_config_menu(app_data_s *ad) {
//
//	Evas_Smart_Cb func[NUM_OF_CONFIG_MENU_ITEMS];
//	func[CONFIG_UPDATE] = on_config_update_cb;
//	func[CONFIG_SUBSCRIBE] = on_config_subscribe_cb;
//	func[CONFIG_PUBLISH] = on_config_publish_cb;
//
//	menu_data_s menu;
//	menu.title_text = get_config_menu_title;
//	menu.item_content = get_config_menu_content;
//	menu.num_items = NUM_OF_CONFIG_MENU_ITEMS;
//	menu.func = func;
//
//	Evas_Object *frame = create_frame(&(ad->frame_config), &menu, ad);
//	elm_naviframe_item_push(ad->nf, NULL, NULL, NULL, frame, "empty");
//}

//
//static void
//on_mqtt_address_cb(void *data, Evas_Object *obj, void *event_info) {
//	app_data_s *ad = (app_data_s*)data;
//	char *broker;
//	bundle_get_str(ad->settings, RAPROTO_SETTING_MQTT_BROKER, &broker);
//	return create_entry(ad, ELM_INPUT_PANEL_LAYOUT_URL, broker, return_broker);
//}
//
//static void
//on_mqtt_username_cb(void *data, Evas_Object *obj, void *event_info) {
//	app_data_s *ad = (app_data_s*)data;
//	char *username;
//	bundle_get_str(ad->settings, RAPROTO_SETTING_MQTT_USERNAME, &username);
//	return create_entry(ad, ELM_INPUT_PANEL_LAYOUT_PASSWORD, username, return_username);
//}
//
//static void
//on_mqtt_password_cb(void *data, Evas_Object *obj, void *event_info) {
//	app_data_s *ad = (app_data_s*)data;
//	char *password;
//	bundle_get_str(ad->settings, RAPROTO_SETTING_MQTT_PASSWORD, &password);
//	return create_entry(ad, ELM_INPUT_PANEL_LAYOUT_PASSWORD, password, return_password);
//}
//
//
//static void
//create_mqtt_menu(app_data_s *ad) {
//	Evas_Smart_Cb func[NUM_OF_MQTT_MENU_ITEMS];
//	func[MQTT_BROKER] = on_mqtt_address_cb;
//	func[MQTT_USERNAME] = on_mqtt_username_cb;
//	func[MQTT_PASSWORD] = on_mqtt_password_cb;
//
//	menu_data_s menu;
//	menu.title_text = get_mqtt_menu_title;
//	menu.item_content = get_mqtt_menu_content;
//	menu.num_items = NUM_OF_MQTT_MENU_ITEMS;
//	menu.func = func;
//
//	Evas_Object *frame = create_frame(&(ad->frame_mqtt), &menu, ad);
//	elm_naviframe_item_push(ad->nf, NULL, NULL, NULL, frame, "empty");
//}

//
//static void
//on_version_cb(void *data, Evas_Object *obj, void *event_info) {
//	app_data_s *ad = (app_data_s*)data;
//	ad->service.reset_taps++;
//	if (ad->service.reset_taps >= RAPROTO_ERROR_RESET_TAPS) {
//		ad->service.reset = true;
//		launch_service_task(RAPROTO_TASK_REQUEST_FACTORY_RESET, ad);
//		ui_app_exit();
//	}
//	return;
//}
//
//static void
//on_info_cb(void *data, Evas_Object *obj, void *event_info) {
//	app_data_s *ad = (app_data_s*)data;
//	return create_text_display(ad,info_message);
//}
//
//static void
//on_disclaimer_cb(void *data, Evas_Object *obj, void *event_info) {
//	app_data_s *ad = (app_data_s*)data;
//	return create_text_display(ad,disclaimer_message);
//}
//
//static void
//on_help_cb(void *data, Evas_Object *obj, void *event_info) {
//	app_data_s *ad = (app_data_s*)data;
//	return create_text_display(ad,help_message);
//}
//
//
//static void
//create_about_menu(app_data_s *ad) {
//	Evas_Smart_Cb func[NUM_OF_ABOUT_MENU_ITEMS];
//	func[ABOUT_VERSION] = on_version_cb;
//	func[ABOUT_INFO] = on_info_cb;
//	func[ABOUT_DISCLAIMER] = on_disclaimer_cb;
//	func[ABOUT_HELP] = on_help_cb;
//
//	menu_data_s menu;
//	menu.title_text = get_about_menu_title;
//	menu.item_content = get_about_menu_content;
//	menu.num_items = NUM_OF_ABOUT_MENU_ITEMS;
//	menu.func = func;
//
//	Evas_Object *frame = create_frame(&(ad->frame_about), &menu, ad);
//	elm_naviframe_item_push(ad->nf, NULL, NULL, NULL, frame, "empty");
//
//	ad->service.reset_taps = 0;
//}


//
//static void
//on_mqtt_cb(void *data, Evas_Object *obj, void *event_info) {
//	app_data_s *ad = (app_data_s*)data;
//	return create_mqtt_menu(ad);
//}
//
//static void
//on_config_cb(void *data, Evas_Object *obj, void *event_info) {
//	app_data_s *ad = (app_data_s*)data;
//	return create_config_menu(ad);
//}
//
//
//static void
//create_settings_menu(app_data_s *ad) {
//
//	Evas_Smart_Cb func[NUM_OF_SETTINGS_MENU_ITEMS];
//	func[SETTINGS_CONFIG] = on_config_cb;
//	func[SETTINGS_MQTT] = on_mqtt_cb;
//
//	menu_data_s menu;
//	menu.title_text = get_settings_menu_title;
//	menu.item_content = get_settings_menu_content;
//	menu.num_items = NUM_OF_SETTINGS_MENU_ITEMS;
//	menu.func = func;
//
//	Evas_Object *frame = create_frame(&(ad->frame_settings), &menu, ad);
//	elm_naviframe_item_push(ad->nf, NULL, NULL, NULL, frame, "empty");
//}


//
//static void
//on_about_cb(void *data, Evas_Object *obj, void *event_info) {
//	app_data_s *ad = (app_data_s*)data;
//	return create_about_menu(ad);
//}
//
//
//static void
//on_settings_cb(void *data, Evas_Object *obj, void *event_info) {
//	app_data_s *ad = (app_data_s*)data;
//	if (ad->service.logging) return;
//
//	return create_settings_menu(ad);
//}
//
//static void
//on_sync_cb(void *data, Evas_Object *obj, void *event_info) {
//	app_data_s *ad = (app_data_s*)data;
//	launch_service_task(RAPROTO_TASK_REQUEST_TRANSMIT, ad);
//	return;
//}
//
//
//static void
//on_onoff_cb(void *data, Evas_Object *obj, void *event_info) {
//	//app_data_s *ad = (app_data_s*)data;
//	return;
//}
//
//
////static void
////on_subject_id_cb(void *data, Evas_Object *obj, void *event_info) {
////	app_data_s *ad = (app_data_s*)data;
////	char *id;
////	bundle_get_str(ad->settings, RAPROTO_SETTING_DEVICE_ID, &id);
////	return create_entry(ad, ELM_INPUT_PANEL_LAYOUT_NUMBERONLY, id, return_subject_id);
////}
//
//
//
//
//
///*
// * Draw Raproto's main menu
// */
//static void
//create_main_menu(app_data_s *ad) {
//	Elm_Object_Item *nf_it;
//
//	Evas_Smart_Cb func[NUM_OF_MAIN_MENU_ITEMS];
//	func[MAIN_MENU_DEVICE_ID] = NULL;
//	func[MAIN_MENU_ONOFF] = on_onoff_cb;
//	func[MAIN_MENU_SYNC] = on_sync_cb;
//	func[MAIN_MENU_SETTINGS] = on_settings_cb;
//	func[MAIN_MENU_ABOUT] = on_about_cb;
//
//	menu_data_s menu;
//	menu.title_text = get_main_menu_title;
//	menu.item_content = get_main_menu_content;
//	menu.num_items = NUM_OF_MAIN_MENU_ITEMS;
//	menu.func = func;
//
//	Evas_Object *frame = create_frame(&(ad->frame_main), &menu, ad);
//	nf_it = elm_naviframe_item_push(ad->nf, NULL, NULL, NULL, frame, "empty");
//	elm_naviframe_item_pop_cb_set(nf_it, win_delete_request_cb, ad->win);
//}



//void
//create_base_gui(app_data_s *ad) {
//	// Window
//	ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
//	elm_win_autodel_set(ad->win, EINA_TRUE);
//
//	if (elm_win_wm_rotation_supported_get(ad->win)) {
//		int rots[4] = { 0, 90, 180, 270 };
//		elm_win_wm_rotation_available_rotations_set(ad->win, (const int *)(&rots), 4);
//	}
//
//	// Conformant
//	ad->conform = elm_conformant_add(ad->win);
//	elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
//	elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);
//	evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
//	elm_win_resize_object_add(ad->win, ad->conform);
//	evas_object_show(ad->conform);
//
//	// Eext Circle Surface Creation
//	ad->circle_surface = eext_circle_surface_conformant_add(ad->conform);
//
//	// Base Layout
//	ad->layout = elm_layout_add(ad->conform);
//	evas_object_size_hint_weight_set(ad->layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
//	elm_layout_theme_set(ad->layout, "layout", "application", "default");
//	evas_object_show(ad->layout);
//	elm_object_content_set(ad->conform, ad->layout);
//
//	// Naviframe
//	ad->nf = elm_naviframe_add(ad->layout);
//	elm_object_part_content_set(ad->layout, "elm.swallow.content", ad->nf);
//	elm_naviframe_event_enabled_set(ad->nf, EINA_TRUE);
//	eext_object_event_callback_add(ad->nf, EEXT_CALLBACK_BACK, eext_naviframe_back_cb, NULL);
//	eext_object_event_callback_add(ad->nf, EEXT_CALLBACK_MORE, eext_naviframe_more_cb, NULL);
//
//	// create the main menu
//	create_main_menu(ad);
//
//	// Show window after base gui is set up
//	evas_object_show(ad->win);
//}

static bool
app_create(void *data) {
	app_data_s *ad = (app_data_s*)data;
	dlog_print(DLOG_INFO, LOG_TAG, "app create");
	ad->settings = NULL;
	ad->service.reboot = false;
	ad->service.error = false;
	ad->service.logging = false;
	ad->service.reset = false;
	launch_service_start(ad);
	return true;
}

static void
app_control(app_control_h app_control, void *data) {
	/* Handle the launch request. */
}

static void
app_pause(void *data) {
	//app_data_s *ad = (app_data_s*)data;

	dlog_print(DLOG_INFO, LOG_TAG, "pause");

	ui_app_exit();

}


static void
app_resume(void *data) {
	//dlog_print(DLOG_INFO, LOG_TAG, "resume");
}


static void
app_terminate(void *data) {
	app_data_s *ad = (app_data_s*)data;
	int err;

	if (!(ad->service.logging) && !(ad->service.reset)) launch_service_task(RAPROTO_TASK_REQUEST_STOP_SOFT, ad); // stop service if not logging

	if ((err = app_control_destroy(ad->service.control)) != APP_CONTROL_ERROR_NONE) return error_msg(err,__func__,"destroy");

	bundle_free(ad->settings);

	ecore_main_loop_quit();
	dlog_print(DLOG_INFO, LOG_TAG, "app terminate");
}


static void
ui_app_lang_changed(app_event_info_h event_info, void *user_data) {
	/*APP_EVENT_LANGUAGE_CHANGED*/
	//	char *locale = NULL;
	//	system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
	//	elm_language_set(locale);
	//	free(locale);
}

static void
ui_app_orient_changed(app_event_info_h event_info, void *user_data) {
	/*APP_EVENT_DEVICE_ORIENTATION_CHANGED*/
}

static void
ui_app_region_changed(app_event_info_h event_info, void *user_data) {
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
ui_app_low_battery(app_event_info_h event_info, void *user_data) {
	/*APP_EVENT_LOW_BATTERY*/
	// TODO: implement
}

static void
ui_app_low_memory(app_event_info_h event_info, void *user_data) {
	/*APP_EVENT_LOW_MEMORY*/
	// TODO: implement
}

int
main(int argc, char *argv[])
{
	app_data_s ad = {0,};
	int err;

	/* Life cycle callback structure */
	ui_app_lifecycle_callback_s event_callback = {0,};
	/* Event handler */
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, ui_app_low_battery, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, ui_app_low_memory, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, ui_app_orient_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, ui_app_region_changed, &ad);


	event_handler_h launch_task_h;
	if ((err = event_add_event_handler(RAPROTO_LAUNCH_TASK_EVENT, launch_service_task_event, &ad, &launch_task_h)) != EVENT_ERROR_NONE) error_msg(err, __func__, "task event");

	event_handler_h settings_h;
	if ((err = event_add_event_handler(RAPROTO_TOPIC_SETTINGS, update_settings, &ad, &settings_h)) != EVENT_ERROR_NONE) error_msg(err, __func__, "settings");

	if ((err = ui_app_main(argc, argv, &event_callback, &ad)) != APP_ERROR_NONE) error_msg(err, __func__, "app main");

	return err;
}
