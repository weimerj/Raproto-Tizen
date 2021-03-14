#include "raprotoservice.h"



void
service_app_terminate(void *data)
{

	dlog_print(DLOG_INFO, LOG_TAG, "app terminate called");

    app_data_s* ad = (app_data_s*)data;

    // Stop logging
    log_stop(ad);

    if (!(ad->reset)) {
    		// Store all non-transmitted data (Handled by log_stop above)
    		//log_message_pack_all(ad,0);
    		//data_save(ad->data.messages, RAPROTO_DATA_FILENAME, NULL);

    		// Save all settings
    		data_save(ad->settings, RAPROTO_SETTING_FILENAME, NULL);
    } else {

		char path[MAX_FILE_PATH];

		bundle_free(ad->settings);
		ad->settings = NULL;
		sprintf(path,"%s%s\n",app_get_data_path(), RAPROTO_SETTING_FILENAME);
		remove(path);

		log_message_pack_all(ad,0);
		bundle_free(ad->data.messages);
		ad->data.messages = NULL;
		sprintf(path,"%s%s\n",app_get_data_path(), RAPROTO_DATA_FILENAME);
		remove(path);
    }

    // free MQTT
	if (ad->mqtt.client != NULL) {
		MQTTAsync_destroy(ad->mqtt.client);
		free(ad->mqtt.client);
		ad->mqtt.client = NULL;
	}

	// free WIFI
    wifi_autoconnect_destroy(&(ad->wifi_ac));
    wifi_deinitialize(ad);

    	// release power lock
    device_power_release_lock(POWER_LOCK_CPU);

    dlog_print(DLOG_INFO, LOG_TAG, "app terminate");
}


void
service_stop_cb(const char *event_name, bundle *settings, void *data){
	//app_data_s *ad = (app_data_s*)data;
	service_app_exit();
}

bool
service_app_create(void *data)
{
	int err;
	dlog_print(DLOG_INFO, LOG_TAG, "app create");

	app_data_s* ad = (app_data_s*)data;

    // factory reset false
	ad->reset = false;
	ad->logging = false;
	ad->display = true;

	// settings initialization
	ad->settings = data_load_settings();

	// Initialize monitors (must be done after loading settings)
	monitor_init(ad);

	// Task Scheduler initializations
	task_init(ad);

	// Wifi initializations
	ad->wifi = NULL;
	wifi_initialize(ad);

	// MQTT initializations
    ad->mqtt.client = NULL;

    // message initialization
    ad->data.initialized = false;
    ad->data.messages = data_load_data();
    utility_log_amount_of_data(ad);


    if ((err = device_power_request_lock(POWER_LOCK_CPU, 0)) != DEVICE_ERROR_NONE) error_msg(err, __func__, "lock CPU");

	return true;
}


static void
service_app_factory_reset(app_data_s *ad){
	ad->reset = true;
	service_app_exit();
}



static void
service_app_control(app_control_h request, void *data)
{
	/* APP_EVENT_CONTROL */
	app_data_s *ad = (app_data_s*)data;

	int err;
	app_control_h reply;
	char *operation = NULL;
	char *task = NULL;

	if ((err = app_control_get_operation(request, &operation)) != APP_CONTROL_ERROR_NONE) error_msg(err, __func__, "get operation");

	if(!strcmp(operation, APP_CONTROL_OPERATION_DEFAULT)) {

		if ((err = app_control_get_extra_data(request, RAPROTO_TASK_REQUEST, &task)) != APP_CONTROL_ERROR_NONE) error_msg(err, __func__, "get extra data");
		if (strcmp(task,RAPROTO_TASK_REQUEST_LOG_CHECK)) dlog_print(DLOG_INFO, LOG_TAG, "Scheduling task: %s", task);


		if (!strcmp(task, RAPROTO_TASK_REQUEST_START)) {
			//pass -- do nothing
		} else if (!strcmp(task, RAPROTO_TASK_REQUEST_STOP)) {
			log_stop(ad);
			service_app_exit();
		} else if (!strcmp(task, RAPROTO_TASK_REQUEST_SETTINGS_TRANSMIT)) {
			data_load_user(RAPROTO_SETTING_DEVICE_ID, request, ad);
			data_load_user(RAPROTO_SETTING_MQTT_BROKER, request, ad);
			data_load_user(RAPROTO_SETTING_MQTT_USERNAME, request, ad);
			data_load_user(RAPROTO_SETTING_MQTT_PASSWORD, request, ad);
			data_load_user(RAPROTO_SETTING_MQTT_CONFIG_PUB_TOPIC, request, ad);
			data_load_user(RAPROTO_SETTING_MQTT_CONFIG_SUB_TOPIC, request, ad);
		} else if (!strcmp(task, RAPROTO_TASK_REQUEST_ERROR)) {
			if (!(ad->reset)) error_msg(APP_CONTROL_ERROR_NONE, __func__, "error request from main app");
		} else if (!strcmp(task, RAPROTO_TASK_REQUEST_FACTORY_RESET)) {
			service_app_factory_reset(ad);
		} else if (!strcmp(task, RAPROTO_TASK_REQUEST_LOG_ON)) {
			ad->scheduler.tasks[RAPROTO_TASK_LOG_ON] = RAPROTO_TASK_SCHEDULE_ON;
		} else if (!strcmp(task, RAPROTO_TASK_REQUEST_LOG_OFF)) {
			ad->scheduler.tasks[RAPROTO_TASK_LOG_OFF] = RAPROTO_TASK_SCHEDULE_ON;
		} else if (!strcmp(task, RAPROTO_TASK_REQUEST_LOG_CHECK)){
			if (!monitor_cb(ad)) return;
		} else if (!strcmp(task, RAPROTO_TASK_REQUEST_SETTINGS_UPDATE)) {
			config_status(RAPROTO_UPDATE_INPROGRESS_MESSAGE, ad->settings);
			ad->scheduler.tasks[RAPROTO_TASK_UPDATE] = RAPROTO_TASK_SCHEDULE_ON;
		} else if (!strcmp(task, RAPROTO_TASK_REQUEST_TRANSMIT)) {
			ad->scheduler.tasks[RAPROTO_TASK_TRANSMIT] = RAPROTO_TASK_SCHEDULE_ON;
		} else if (!strcmp(task, RAPROTO_TASK_REQUEST_STOP_SOFT)) {
			ad->scheduler.tasks[RAPROTO_TASK_STOP_SOFT] = RAPROTO_TASK_SCHEDULE_ON;
		} else if (!strcmp(task, RAPROTO_TASK_REQUEST_CLEAR_QUEUE)){
			task_init(ad);
		} else {
			error_msg(BUNDLE_ERROR_INVALID_PARAMETER, __func__, "unknown task");
		}

		if ((err = app_control_create(&reply)) != APP_CONTROL_ERROR_NONE) return error_msg(err,__func__,"create");
		if ((err = app_control_set_app_id(reply, RAPROTO_APP_ID)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "app_id");
		if ((err = app_control_reply_to_launch_request(reply, request, APP_CONTROL_RESULT_SUCCEEDED)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__,"reply");
		if ((err = app_control_destroy(reply)) != APP_CONTROL_ERROR_NONE) return error_msg(err,__func__,"destroy");

		if (ad->display) {
			refresh_display(ad->settings);
			ad->display = false;
		}

		task_start(ad);
	}

	return;
}


static void
service_app_lang_changed(app_event_info_h event_info, void *data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
}


static void
service_app_region_changed(app_event_info_h event_info, void *data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
service_app_low_battery(app_event_info_h event_info, void *data)
{
	app_data_s *ad = (app_data_s*)data;
	int err;
	app_event_low_battery_status_e status;
	dlog_print(DLOG_WARN, LOG_TAG, "Low battery called");

	if ((err = app_event_get_low_battery_status(event_info, 	&status)) != APP_ERROR_NONE) error_msg(err,__func__, "get low battery");

	if (status == APP_EVENT_LOW_BATTERY_POWER_OFF){
		log_stop(ad);
		service_app_exit();
	}

	/*APP_EVENT_LOW_BATTERY*/
}

static void
service_app_low_memory(app_event_info_h event_info, void *data)
{
	app_data_s *ad = (app_data_s*)data;
	dlog_print(DLOG_ERROR, LOG_TAG, "Low memory called");

	log_stop(ad);
	service_app_exit();
	/*APP_EVENT_LOW_MEMORY*/
}

int main(int argc, char* argv[])
{
	int err;

	// initialize application data
    app_data_s ad = {0,};

	service_app_lifecycle_callback_s event_callback;
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = service_app_create;
	event_callback.terminate = service_app_terminate;
	event_callback.app_control = service_app_control;

	service_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, service_app_low_battery, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, service_app_low_memory, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, service_app_lang_changed, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, service_app_region_changed, &ad);


	event_handler_h task_start_h;
	if ((err = event_add_event_handler(RAPROTO_TASK_START, task_process, &ad, &task_start_h)) != EVENT_ERROR_NONE) error_msg(err, __func__,"start");

	event_handler_h task_monitor_on_h, task_monitor_off_h;
	if ((err = event_add_event_handler(RAPROTO_TASK_MONITOR_ON_DONE, task_process, &ad, &task_monitor_on_h)) != EVENT_ERROR_NONE) error_msg(err, __func__,"task monitor on");
	if ((err = event_add_event_handler(RAPROTO_TASK_MONITOR_OFF_DONE, task_process, &ad, &task_monitor_off_h)) != EVENT_ERROR_NONE) error_msg(err, __func__,"task monitor off");

	event_handler_h task_wifi_on_h, task_wifi_off_h;
	if ((err = event_add_event_handler(RAPROTO_TASK_WIFI_ON_DONE, task_process, &ad, &task_wifi_on_h)) != EVENT_ERROR_NONE) error_msg(err, __func__, "wifi on");
	if ((err = event_add_event_handler(RAPROTO_TASK_WIFI_OFF_DONE, task_process, &ad, &task_wifi_off_h)) != EVENT_ERROR_NONE) error_msg(err, __func__, "wifi off");

	event_handler_h task_mqtt_on_h, task_mqtt_off_h, task_mqtt_destroy_h;
	if ((err = event_add_event_handler(RAPROTO_TASK_MQTT_ON_DONE, task_process, &ad, &task_mqtt_on_h)) != EVENT_ERROR_NONE) error_msg(err, __func__, "mqtt on");
	if ((err = event_add_event_handler(RAPROTO_TASK_MQTT_OFF_DONE, task_process, &ad, &task_mqtt_off_h)) != EVENT_ERROR_NONE) error_msg(err, __func__, "mqtt off");
	if ((err = event_add_event_handler(RAPROTO_TASK_MQTT_DESTROY_DONE, task_process, &ad, &task_mqtt_destroy_h)) != EVENT_ERROR_NONE) error_msg(err, __func__, "mqtt destroy");

	event_handler_h task_transmit_h, task_transmit_setup_h, task_config_h, task_log_h;
	if ((err = event_add_event_handler(RAPROTO_TASK_TRANSMIT_DONE, task_process, &ad, &task_transmit_h)) != EVENT_ERROR_NONE) error_msg(err, __func__, "transmit");
	if ((err = event_add_event_handler(RAPROTO_TASK_TRANSMIT_SETUP_DONE, task_process, &ad, &task_transmit_setup_h)) != EVENT_ERROR_NONE) error_msg(err, __func__, "transmit");
	if ((err = event_add_event_handler(RAPROTO_TASK_CONFIG_DONE, task_process, &ad, &task_config_h)) != EVENT_ERROR_NONE) error_msg(err, __func__, "config");
	if ((err = event_add_event_handler(RAPROTO_TASK_LOG_DONE, task_process, &ad, &task_log_h)) != EVENT_ERROR_NONE) error_msg(err, __func__, "monitor");

	event_handler_h error_event_h;
	if ((err = event_add_event_handler(RAPROTO_ERROR_EVENT, error_msg_event, &ad, &error_event_h)) != EVENT_ERROR_NONE) error_msg(err, __func__,"start");

	return service_app_main(argc, argv, &event_callback, &ad);

}
