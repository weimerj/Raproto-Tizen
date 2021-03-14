/*
 * log.c
 *
 *  Created on: Feb 14, 2021
 *      Author: weimer
 */



#include "log.h"





static unsigned long long
log_gettime(){

	struct timespec ts;
    unsigned long long ms1970; // Milliseconds since Jan 1, 1970

	clock_gettime(CLOCK_REALTIME, &ts);
	ms1970 = ts.tv_sec;
	ms1970 = (ms1970 * 1000) + round(ts.tv_nsec / 1.0e6);

	return ms1970;

}




void
log_message_pack_all(app_data_s *ad, int attempt) {
	// load settings
	int err;
	char key[RAPROTO_MAX_KEY_SIZE];


	// implement mutex
	while(ad->data.mutex != 0) {
		usleep(RAPROTO_MUTEX_SLEEP);
	}
	ad->data.mutex++;
	if (ad->data.mutex != 1){
		warn_msg(APP_CONTROL_ERROR_IO_ERROR, __func__, "mutex collision");
		ad->data.mutex--;
		attempt++;
		if (attempt <= RAPROTO_MUTEX_ATTEMPTS) log_message_pack_all(ad, attempt);
		return;
	}

	if (ad->data.payload != NULL){
		if (strlen(ad->data.payload) > 0) {

			int *temp;
			size_t n_size;
			if ((err = bundle_get_byte(ad->data.messages, RAPROTO_DATA_IDX_END, (void**)&temp, &n_size)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "retrieving idx end");
			int idx_end = *temp;

			increment_queue_idx(&idx_end, 1, RAPROTO_DATA_MAX_IDX);
			if ((err = bundle_del(ad->data.messages, RAPROTO_DATA_IDX_END)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "deleting idx end");
			if ((err = bundle_add_byte(ad->data.messages, RAPROTO_DATA_IDX_END, &idx_end, sizeof(int))) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "updating idx end");

			sprintf(key, "%d", idx_end);
			if ((err = bundle_add_str(ad->data.messages, key, ad->data.payload)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "adding payload to bundle");

			dlog_print(DLOG_INFO, LOG_TAG, "message %s: len = %d ", key, strlen(ad->data.payload));
			utility_log_amount_of_data(ad);

			memset(ad->data.payload, 0, (ad->data.payloadmax) * sizeof(char));
		}
	}

	//dlog_print(DLOG_INFO, LOG_TAG, "%s", ad->data.payload);
	ad->data.mutex--;

}





static void
log_message_pack(app_data_s *ad, char *str, int attempt){

	// load settings
	int err;
	char key[RAPROTO_MAX_KEY_SIZE];

	// implement mutex
	while(ad->data.mutex != 0) {
		usleep(RAPROTO_MUTEX_SLEEP);
	}
	ad->data.mutex++;
	if (ad->data.mutex != 1){
		dlog_print(DLOG_WARN, LOG_TAG, "mutex violation %d: %s", attempt, str); //TODO: add as warning
		ad->data.mutex--;
		attempt++;
		if (attempt <= RAPROTO_MUTEX_ATTEMPTS) log_message_pack(ad, str, attempt);
		return;
	}

	// Prepare for thingsboard (must start with '[')
	if (strlen(ad->data.payload) == 0){
		ad->data.payload[0] = '[';
		ad->data.payload[1] = '\0';
	} else if (strlen(ad->data.payload) >= ad->data.payloadmax - RAPROTO_MAX_MESSAGE_SIZE - 1){
		int *temp;
		size_t n_size;
		if ((err = bundle_get_byte(ad->data.messages, RAPROTO_DATA_IDX_END, (void**)&temp, &n_size)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "retrieving idx end");
		int idx_end = *temp;

		increment_queue_idx(&idx_end, 1, RAPROTO_DATA_MAX_IDX);
		if ((err = bundle_del(ad->data.messages, RAPROTO_DATA_IDX_END)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "deleting idx end");
		if ((err = bundle_add_byte(ad->data.messages, RAPROTO_DATA_IDX_END, &idx_end, sizeof(int))) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "updating idx end");

		sprintf(key, "%d", idx_end);
		if ((err = bundle_add_str(ad->data.messages, key, ad->data.payload)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "adding payload to bundle");

		dlog_print(DLOG_INFO, LOG_TAG, "message %s: len = %d ", key, strlen(ad->data.payload));
		utility_log_amount_of_data(ad);

		memset(ad->data.payload, 0, (ad->data.payloadmax) * sizeof(char));
		ad->data.payload[0] = '[';
		ad->data.payload[1] = '\0';
	} else{
		ad->data.payload[strlen(ad->data.payload)-1] = ',';
	}

	strcpy(&(ad->data.payload[strlen(ad->data.payload)]), str);

	ad->data.payload[strlen(ad->data.payload)] = ']'; // must end with ']' for thingsboard
	ad->data.payload[strlen(ad->data.payload)] = '\0'; // insert null character

	//dlog_print(DLOG_INFO, LOG_TAG, "%s", ad->data.payload);
	ad->data.mutex--;
}


void
log_stop(app_data_s *ad){

	int err;
	int val;

	if ((err = bundle_del(ad->settings, RAPROTO_SETTING_LOGGING)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "del logging");
	val = RAPROTO_SETTING_LOGGING_FALSE;
	if ((err = bundle_add_byte(ad->settings, RAPROTO_SETTING_LOGGING, &val, sizeof(int))) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "add log byte");
	ad->logging = false;

	for (int j = 0; j < RAPROTO_SENSOR_MAX_NUM; j++){
		if ((ad->sensors[j].monitor.on) && (j != RAPROTO_SENSOR_AUTOSAVE) && (j != RAPROTO_SENSOR_AUTOTRANSMIT) && (j != RAPROTO_SENSOR_BATTERY)) {
			if ((err = sensor_destroy_listener(ad->sensors[j].listener)) != SENSOR_ERROR_NONE) error_msg(err, __func__, "listener destroy");
		}
		monitor_control(j, 1, RAPROTO_MONITOR_STATE_OFF, ad);
	}

	log_message_pack_all(ad,0);
	data_save(ad->data.messages, RAPROTO_DATA_FILENAME, NULL);

}



static Eina_Bool
log_sensor_system(void *data){

	app_data_s *ad = (app_data_s*)data;
	char *device_id;
	int err;
	int battery_percent;
	char msg[RAPROTO_MAX_MESSAGE_SIZE];

	if (monitor_heart_beat(RAPROTO_SENSOR_BATTERY,ad)) {
		if ((err = bundle_get_str(ad->settings, RAPROTO_SETTING_DEVICE_ID, &device_id)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "device id");

		if ((err = device_battery_get_percent(&battery_percent)) != DEVICE_ERROR_NONE) error_msg(err, __func__, "get percentage");

		sprintf(msg, "{\"ts\": \"%llu\",\"values\"={\"%s_SYS\":{\"Bat\":%d}}}", log_gettime(), device_id, battery_percent);

		log_message_pack(ad, msg, 0);

		return ECORE_CALLBACK_RENEW;
	} else {
		return ECORE_CALLBACK_CANCEL;
	}


}


static void
log_sensor_gravity(sensor_h sensor, sensor_event_s *event, int events_count, void *data)
{
	app_data_s *ad = (app_data_s*)data;
	char *device_id;
	char msg[RAPROTO_MAX_MESSAGE_SIZE];
	int err;
	unsigned long long int offset = log_gettime();

	if (monitor_heart_beat(RAPROTO_SENSOR_GRAVITY, ad)) {
		if ((err = bundle_get_str(ad->settings, RAPROTO_SETTING_DEVICE_ID, &device_id)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "device id");

		for (int j = 0; j < events_count; j++){
			sensor_type_e type = SENSOR_ALL;
			if((sensor_get_type(sensor, &type) == SENSOR_ERROR_NONE) && type == SENSOR_GRAVITY)
			{
				sprintf(msg, "{\"ts\": \"%llu\",\"values\"={\"%s_GRAVITY\":{\"x\":%.2f,\"y\":%.2f,\"z\":%.2f}}}",
					offset + (event[j].timestamp - event[events_count-1].timestamp)/1000,
					device_id,
					event->values[0],
					event->values[1],
					event->values[2]);
			}

			log_message_pack(ad, msg, 0);
		}
	}
}



static void
log_sensor_green_light(sensor_h sensor, sensor_event_s *event, int events_count, void *data)
{
	app_data_s *ad = (app_data_s*)data;
	char *device_id;
	char msg[RAPROTO_MAX_MESSAGE_SIZE];
	int err;
	unsigned long long int offset = log_gettime();

	if (monitor_heart_beat(RAPROTO_SENSOR_HRM_GREEN, ad)) {
		if ((err = bundle_get_str(ad->settings, RAPROTO_SETTING_DEVICE_ID, &device_id)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "device id");

		for (int j = 0; j < events_count; j++){
			sensor_type_e type = SENSOR_ALL;
			if((sensor_get_type(sensor, &type) == SENSOR_ERROR_NONE) && type == SENSOR_HRM_LED_GREEN)
			{
				sprintf(msg, "{\"ts\": \"%llu\",\"values\"={\"%s_LED_GREEN\":{\"val\":%.1f}}}",
					offset + (event[j].timestamp - event[events_count-1].timestamp)/1000,
					device_id,
					event->values[0]);
			}

			log_message_pack(ad, msg, 0);
		}
	}
}




static void
log_sensor_gyroscope(sensor_h sensor, sensor_event_s *event, int events_count, void *data)
{
	app_data_s *ad = (app_data_s*)data;
	char *device_id;
	char msg[RAPROTO_MAX_MESSAGE_SIZE];
	int err;
	unsigned long long int offset = log_gettime();

	if (monitor_heart_beat(RAPROTO_SENSOR_GYRO, ad)) {
		if ((err = bundle_get_str(ad->settings, RAPROTO_SETTING_DEVICE_ID, &device_id)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "device id");

		for (int j = 0; j < events_count; j++){
			sensor_type_e type = SENSOR_ALL;
			if((sensor_get_type(sensor, &type) == SENSOR_ERROR_NONE) && type == SENSOR_GYROSCOPE)
			{
				sprintf(msg, "{\"ts\": \"%llu\",\"values\"={\"%s_GYRO\":{\"x\":%.2f,\"y\":%.2f,\"z\":%.2f}}}",
					offset + (event[j].timestamp - event[events_count-1].timestamp)/1000,
					device_id,
					event->values[0],
					event->values[1],
					event->values[2]);
			}

			log_message_pack(ad, msg, 0);
		}
	}
}



static void
log_sensor_accelerometer(sensor_h sensor, sensor_event_s *event, int events_count, void *data)
{
	app_data_s *ad = (app_data_s*)data;
	char *device_id;
	char msg[RAPROTO_MAX_MESSAGE_SIZE];
	int err;
	unsigned long long int offset = log_gettime();

	if (monitor_heart_beat(RAPROTO_SENSOR_ACC, ad)) {
		if ((err = bundle_get_str(ad->settings, RAPROTO_SETTING_DEVICE_ID, &device_id)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "device id");

		for (int j = 0; j < events_count; j++){
		    sensor_type_e type = SENSOR_ALL;
		    if((sensor_get_type(sensor, &type) == SENSOR_ERROR_NONE) && type == SENSOR_ACCELEROMETER)
		    {

		    		sprintf(msg, "{\"ts\": \"%llu\",\"values\"={\"%s_ACC\":{\"x\":%.2f,\"y\":%.2f,\"z\":%.2f}}}",
		    			offset + (event[j].timestamp - event[events_count-1].timestamp)/1000,
		    			device_id,
					event[j].values[0],
					event[j].values[1],
					event[j].values[2]);
		    }

		    log_message_pack(ad, msg, 0);

		}
	}
}




static void
log_sensor_heart_rate_monitor(sensor_h sensor, sensor_event_s *event, int events_count, void *data)
{
	app_data_s *ad = (app_data_s*)data;
	char *device_id;
	char msg[RAPROTO_MAX_MESSAGE_SIZE];
	int err;
	unsigned long long int offset = log_gettime();

	if (monitor_heart_beat(RAPROTO_SENSOR_HRM, ad)) {
		if ((err = bundle_get_str(ad->settings, RAPROTO_SETTING_DEVICE_ID, &device_id)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "device id");

		for (int j = 0; j < events_count; j++){

			sensor_type_e type = SENSOR_ALL;
			if((sensor_get_type(sensor, &type) == SENSOR_ERROR_NONE) && (type == SENSOR_HRM))
			{
				sprintf(msg, "{\"ts\": \"%lld\",\"values\"={\"%s_HRM\":{\"hrm\":%d}}}",
					offset + (event[j].timestamp - event[events_count-1].timestamp)/1000,
					device_id,
					(int)event->values[0]);
			}

			log_message_pack(ad, msg, 0);
		}
	}
}




static void
log_sensor_setup(int idx, sensor_type_e type, const char *key, sensor_events_cb callback, int mode, app_data_s *ad){

	int err;
	bool sensor_supported = false;


	if ((err = sensor_is_supported(type, &sensor_supported)) != SENSOR_ERROR_NONE) error_msg(err, __func__, "sensor supported");

	if (sensor_supported){

		int *log_period;
		size_t log_period_size;
		if ((err = bundle_get_byte(ad->settings, key, (void**)&log_period, &log_period_size)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get key");

		if (*log_period > 0) {

			unsigned int pos_log = *log_period; // need unsigned for the log period
			dlog_print(DLOG_INFO, LOG_TAG, "Sensor %s logging at rate %u ms", key, pos_log);

			if ((err = sensor_get_default_sensor(type, &(ad->sensors[idx].sensor))) != SENSOR_ERROR_NONE) return error_msg(err, __func__, "sensor_get_default_sensor");
			if ((err = sensor_create_listener(ad->sensors[idx].sensor, &(ad->sensors[idx].listener))) != SENSOR_ERROR_NONE) return error_msg(err, __func__, "sensor_create_listener");
			if ((err = sensor_listener_set_events_cb(ad->sensors[idx].listener, callback, ad)) != SENSOR_ERROR_NONE) return error_msg(err, __func__, "sensor_listener_set_event_cb");
			if ((err = sensor_listener_set_interval(ad->sensors[idx].listener, pos_log)) != SENSOR_ERROR_NONE) return error_msg(err, __func__, "sensor_listener_set_event_cb");
			if ((err = sensor_listener_set_attribute_int(ad->sensors[idx].listener, SENSOR_ATTRIBUTE_AXIS_ORIENTATION, SENSOR_AXIS_DEVICE_ORIENTED)) != SENSOR_ERROR_NONE) return error_msg(err, __func__, "sensor_listener_set_attribute_int 1");
			if ((err = sensor_listener_set_attribute_int(ad->sensors[idx].listener, SENSOR_ATTRIBUTE_PAUSE_POLICY, SENSOR_PAUSE_NONE)) != SENSOR_ERROR_NONE) return error_msg(err, __func__, "sensor_listener_set_attribute_int 2");
			if ((err = sensor_listener_start(ad->sensors[idx].listener)) != SENSOR_ERROR_NONE) return error_msg(err, __func__, "listener start");

			switch (mode){
			case (0):
				monitor_control(idx, 1, RAPROTO_MONITOR_STATE_ON, ad);
				break;
			case (1):
				monitor_control(idx, *log_period, RAPROTO_MONITOR_STATE_ON, ad);
				break;
			default:
				error_msg(APP_CONTROL_ERROR_IO_ERROR, __func__, "invalid mode");
			}

		}
	}
	return;
}



static void
log_timer_setup(int idx, const char *setting, Ecore_Task_Cb func, app_data_s *ad) {
	int *period;
	size_t period_size;
	bundle_get_byte(ad->settings, setting, (void**)&period, &period_size);
	if (*period > 0) {
		ecore_timer_add(*period, func, ad);
		if ((idx != RAPROTO_SENSOR_AUTOSAVE) && (idx != RAPROTO_SENSOR_AUTOTRANSMIT)) {
			monitor_control(idx, *period, RAPROTO_MONITOR_STATE_ON, ad);
		}
	}

	if ((*period <= 0) && (!strcmp(RAPROTO_SETTING_BATTERY, setting))) error_msg(APP_CONTROL_ERROR_IO_ERROR, __func__, "battery must be positive");

}



void
log_start(app_data_s *ad){

	int err;
	int val;

	ecore_init();

	// Setup data
	data_setup(ad);

	// Setup timers
	log_timer_setup(RAPROTO_SENSOR_AUTOTRANSMIT, RAPROTO_SETTING_TX_RATE, transmit_autorequest, ad);
	log_timer_setup(RAPROTO_SENSOR_AUTOSAVE, RAPROTO_SETTING_SAVE_RATE, data_autosave, ad);
	log_timer_setup(RAPROTO_SENSOR_BATTERY, RAPROTO_SETTING_BATTERY, log_sensor_system, ad);

	// Setup sensors
	//TODO: add the other sensors
	log_sensor_setup(RAPROTO_SENSOR_ACC, SENSOR_ACCELEROMETER, RAPROTO_SETTING_SENSOR_ACC, log_sensor_accelerometer, 0, ad);
	log_sensor_setup(RAPROTO_SENSOR_HRM, SENSOR_HRM, RAPROTO_SETTING_SENSOR_HRM, log_sensor_heart_rate_monitor, 1, ad);
	log_sensor_setup(RAPROTO_SENSOR_GRAVITY, SENSOR_GRAVITY, RAPROTO_SETTING_SENSOR_GRAVITY, log_sensor_gravity, 0, ad);
	log_sensor_setup(RAPROTO_SENSOR_GYRO, SENSOR_GYROSCOPE, RAPROTO_SETTING_SENSOR_GYRO, log_sensor_gyroscope, 0, ad);
	log_sensor_setup(RAPROTO_SENSOR_HRM_GREEN, SENSOR_HRM_LED_GREEN, RAPROTO_SETTING_SENSOR_HRM_GREEN, log_sensor_green_light, 0, ad);


	//if ((err = device_display_set_brightness(0,1)) != DEVICE_ERROR_NONE) error_msg(err, __func__, "brightness");
	//if ((err = device_display_change_state(DISPLAY_STATE_SCREEN_OFF)) != DEVICE_ERROR_NONE) error_msg(err, __func__, "screen state");
	//if ((err = device_power_request_lock(POWER_LOCK_CPU, 0)) != DEVICE_ERROR_NONE) error_msg(err, __func__, "lock CPU");

	dlog_print(DLOG_INFO, LOG_TAG, "monitoring started");

	if ((err = bundle_del(ad->settings, RAPROTO_SETTING_LOGGING)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "del logging");
	val = RAPROTO_SETTING_LOGGING_TRUE;
	if ((err = bundle_add_byte(ad->settings, RAPROTO_SETTING_LOGGING, &val, sizeof(int))) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "add log byte");
	ad->logging = true;

	task_process_cb(RAPROTO_TASK_LOG_DONE);

	ecore_main_loop_begin();
}



