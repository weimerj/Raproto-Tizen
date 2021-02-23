#include <raproto.h>


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
