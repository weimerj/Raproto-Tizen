/*
 * task_manager.c
 *
 *  Created on: Feb 14, 2021
 *      Author: weimer
 */


#include "task_manager.h"


static void
task_msg(const char *msg, app_data_s *ad){
	int err;

	switch (ad->scheduler.active){
		case RAPROTO_TASK_UPDATE:
			if ((err = bundle_del(ad->settings, RAPROTO_SETTING_CONFIG_STATUS)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "bundle del config");
			if ((err = bundle_add_str(ad->settings, RAPROTO_SETTING_CONFIG_STATUS, msg)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "config error");
			break;
		case RAPROTO_TASK_LOG_ON:
			break;
		case RAPROTO_TASK_TRANSMIT:
			break;
		case RAPROTO_TASK_NONE:
			return;
		default:
			error_msg(APP_CONTROL_ERROR_KEY_NOT_FOUND, __func__, "unknown task");
			break;
	}

	//config_publish(ad->settings);
	return;
}


void
task_error(const char *msg, app_data_s *ad){

	task_msg(msg, ad);

	error_msg(APP_CONTROL_ERROR_IO_ERROR, __func__, msg);

	return;
}


void
task_warn(const char *msg, app_data_s *ad){

	warn_msg(APP_CONTROL_ERROR_IO_ERROR, __func__, msg);

	task_msg(msg,ad);

	return;
}



static void
task_stop(app_data_s *ad){
	refresh_display(ad->settings); // publish configuration to main app

	dlog_print(DLOG_INFO, LOG_TAG, "task complete: %d", ad->scheduler.active);
	ad->scheduler.tasks[ad->scheduler.active] = RAPROTO_TASK_SCHEDULE_OFF;
	ad->scheduler.active = RAPROTO_TASK_NONE;
	task_start(ad);
	return;
}



void
task_process_cb(const char* event){
	int err;
	bundle *b = bundle_create();
	if ((err = event_publish_app_event(event, b)) != EVENT_ERROR_NONE) error_msg(err,__func__, NULL);
	bundle_free(b);
}


//TODO: Make sure all tasks have a timeout and graceful exit
void
task_process(const char *event_name, bundle *junk, void *data){
	app_data_s *ad = (app_data_s*)data;

	dlog_print(DLOG_INFO, LOG_TAG, "task_process: %d : %s", ad->scheduler.active, event_name);

	switch(ad->scheduler.active){
		case (RAPROTO_TASK_UPDATE):
			if (!strcmp(event_name, RAPROTO_TASK_START))	wifi_start(ad);
			else if (!strcmp(event_name, RAPROTO_TASK_WIFI_ON_DONE)) mqtt_start(ad);
			else if (!strcmp(event_name, RAPROTO_TASK_MQTT_ON_DONE)) config_start(ad);
			else if (!strcmp(event_name, RAPROTO_TASK_CONFIG_DONE)) mqtt_stop(ad);
			else if (!strcmp(event_name, RAPROTO_TASK_MQTT_OFF_DONE)) mqtt_destroy(ad);
			else if (!strcmp(event_name, RAPROTO_TASK_MQTT_DESTROY_DONE)) wifi_stop(ad);
			else if (!strcmp(event_name, RAPROTO_TASK_WIFI_OFF_DONE)) task_stop(ad);
			return;
		case (RAPROTO_TASK_LOG_ON):
			if (!strcmp(event_name, RAPROTO_TASK_START)) log_start(ad);
			else if (!strcmp(event_name, RAPROTO_TASK_LOG_DONE)) wifi_stop(ad);
			else if (!strcmp(event_name, RAPROTO_TASK_WIFI_OFF_DONE)) task_stop(ad);
			return;
		case (RAPROTO_TASK_LOG_OFF):
			log_stop(ad);
			task_stop(ad);
			return;
		case (RAPROTO_TASK_TRANSMIT):
			if (!strcmp(event_name, RAPROTO_TASK_START)) wifi_start(ad);
			else if (!strcmp(event_name, RAPROTO_TASK_WIFI_ON_DONE)) mqtt_start(ad);
			else if (!strcmp(event_name, RAPROTO_TASK_MQTT_ON_DONE)) transmit_data_setup(ad);
			else if (!strcmp(event_name, RAPROTO_TASK_TRANSMIT_SETUP_DONE)) transmit_start(ad);
			else if (!strcmp(event_name, RAPROTO_TASK_TRANSMIT_DONE)) mqtt_stop(ad);
			else if (!strcmp(event_name, RAPROTO_TASK_MQTT_OFF_DONE)) mqtt_destroy(ad);
			else if (!strcmp(event_name, RAPROTO_TASK_MQTT_DESTROY_DONE)) wifi_stop(ad);
			else if (!strcmp(event_name, RAPROTO_TASK_WIFI_OFF_DONE)) task_stop(ad);
			return;
		case (RAPROTO_TASK_STOP_SOFT):
			service_app_exit();
			return;
		default:
			dlog_print(DLOG_ERROR, LOG_TAG, "unknown task number: %d", ad->scheduler.active);
			task_stop(ad);
	}

	return;
}




void
task_start(app_data_s *ad){

	if (ad->scheduler.active == RAPROTO_TASK_NONE) {
		for (int j = 0; j < RAPROTO_TASK_NUMBER; j++){
			if (ad->scheduler.tasks[j] == RAPROTO_TASK_SCHEDULE_ON){
				dlog_print(DLOG_INFO, LOG_TAG, "Starting task: %d", j);
				ad->scheduler.active = j;
				task_process_cb(RAPROTO_TASK_START);
				return;
			}
		}
	} else {
		dlog_print(DLOG_INFO, LOG_TAG, "Task already in process: %d", ad->scheduler.active);
	}
	return;
}



void
task_init(app_data_s *ad){
	for (int j=0; j < RAPROTO_TASK_NUMBER; j++){
		ad->scheduler.tasks[j] = RAPROTO_TASK_SCHEDULE_OFF;
	}
	ad->scheduler.active = RAPROTO_TASK_NONE;
}

