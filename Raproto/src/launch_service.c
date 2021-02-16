/*
 * launch_service.c
 *
 *  Created on: Feb 15, 2021
 *      Author: weimer
 */


#include "launch_service.h"



Eina_Bool
launch_heart_beat(void *data){
	app_data_s *ad = (app_data_s *)data;
	int err;
	bool running;
	time_t cur_time = time(NULL);

	//dlog_print(DLOG_INFO, LOG_TAG, "heart beat");
	if ((err = app_manager_is_running(RAPROTO_SERVICE_APP_ID, &running)) != APP_MANAGER_ERROR_NONE) error_msg(err, __func__, "app running");

	update_theme(ad);

	if (cur_time - ad->service.t > 2*RAPROTO_HEART_BEAT){
		ad->service.error = true;
	}

	if (ad->service.reboot) {
		ad->service.t = cur_time;
	} else if (!(ad->service.error)) {
		if (running) {
			if (ad->settings == NULL) {
				launch_service_task(RAPROTO_TASK_REQUEST_SETTINGS_TRANSMIT, ad);
				ad->service.t = cur_time;
			} else if (ad->service.logging) {
				launch_service_task(RAPROTO_TASK_REQUEST_LOG_CHECK, ad);
			} else {
				ad->service.t = cur_time;
			}
		} else {
			ad->service.error = true;
			launch_service_task(RAPROTO_TASK_REQUEST_ERROR, ad);
		}
	}

	return ECORE_CALLBACK_RENEW;
}



static void
launch_heart_beat_cb(app_control_h request, app_control_h reply, app_control_result_e result, void *data) {
	app_data_s *ad = (app_data_s*)data;
	ad->service.t = time(NULL);
	return;
}



static void
launch_service_start_cb(app_control_h request, app_control_h reply, app_control_result_e result, void *data)
{
	app_data_s *ad = (app_data_s*)data;
	ad->service.t = time(NULL);
	if (!(ad->service.reboot)) {
		ecore_init();
		ecore_timer_add(RAPROTO_HEART_BEAT, launch_heart_beat, ad);
		ecore_main_loop_begin();
	} else {
		ad->service.reboot = false;
	}

}


void
launch_service_start(app_data_s *ad) {
	int err;

	if (ad->service.control == NULL) {
		if ((err = app_control_create(&(ad->service.control))) != APP_CONTROL_ERROR_NONE) return error_msg(err,__func__, "control create");
		if ((err = app_control_set_operation(ad->service.control, APP_CONTROL_OPERATION_DEFAULT)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "set operation");
		if ((err = app_control_set_app_id(ad->service.control, RAPROTO_SERVICE_APP_ID)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "setup app id");
	}
	if ((err = app_control_add_extra_data(ad->service.control, RAPROTO_TASK_REQUEST, RAPROTO_TASK_REQUEST_START)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "extra data");
	if ((err = app_control_send_launch_request(ad->service.control, launch_service_start_cb, ad)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "send launch");

}



static void
launch_service_task_wait_for_response(app_control_h *control, app_control_reply_cb callback, void *user_data) {
	int err;
	while ((err = app_control_send_launch_request(*control, callback, user_data)) != APP_CONTROL_ERROR_NONE) {
		if (err == APP_CONTROL_ERROR_LAUNCH_REJECTED){
			usleep(RAPROTO_MUTEX_SLEEP);
		} else {
			return error_msg(err, __func__, "send launch");
		}
	}
}


static void
launch_service_task_wait_for_terminate(app_data_s *ad) {
	int err;
	bool running;
	ad->service.reboot = true;

	launch_service_task_wait_for_response(&(ad->service.control), NULL, NULL);

	if ((err = app_manager_is_running(RAPROTO_SERVICE_APP_ID, &running)) != APP_MANAGER_ERROR_NONE) error_msg(err, __func__, "app running");
	while (running) {
		usleep(RAPROTO_MUTEX_SLEEP);
		if ((err = app_manager_is_running(RAPROTO_SERVICE_APP_ID, &running)) != APP_MANAGER_ERROR_NONE) error_msg(err, __func__, "app running");
	}

	launch_service_start(ad);
}

void
launch_service_task(char *task, app_data_s *ad){
	int err;
	bool running;

	if (ad->service.control == NULL) error_msg(APP_CONTROL_ERROR_NONE, __func__, "no control");

	if ((err = app_manager_is_running(RAPROTO_SERVICE_APP_ID, &running)) != APP_MANAGER_ERROR_NONE) error_msg(err, __func__, "app running");

	if ((!(ad->service.error)) || (!strcmp(task,RAPROTO_TASK_REQUEST_FACTORY_RESET))) {
		if (running) {
			if ((err = app_control_remove_extra_data(ad->service.control, RAPROTO_TASK_REQUEST)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "remove extra data");
			if ((err = app_control_add_extra_data(ad->service.control, RAPROTO_TASK_REQUEST, task)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "add extra data");

			if (!strcmp(task,RAPROTO_TASK_REQUEST_LOG_CHECK)) {
				launch_service_task_wait_for_response(&(ad->service.control), launch_heart_beat_cb, (void*)ad);
			} else if (!strcmp(task, RAPROTO_TASK_REQUEST_LOG_OFF)) {
				launch_service_task_wait_for_terminate(ad);
			}	else {
				launch_service_task_wait_for_response(&(ad->service.control), NULL, NULL);
			}
		} else {
			ad->service.error = true;
		}
	}
}


void
launch_service_task_event(const char *event_name, bundle *task_b, void *data){
	app_data_s *ad = (app_data_s*)data;
	int err;
	char *task;

	if ((err = bundle_get_str(task_b, RAPROTO_TASK_REQUEST, &task)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get str");
	launch_service_task(task, ad);

}



