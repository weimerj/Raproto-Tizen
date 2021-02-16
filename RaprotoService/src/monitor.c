/*
 * monitor.c
 *
 *  Created on: Feb 14, 2021
 *      Author: weimer
 */


#include "monitor.h"


bool
monitor_heart_beat(int idx, app_data_s *ad){
	ad->sensors[idx].monitor.t = time(NULL);
	return ad->sensors[idx].monitor.on;
}


void
monitor_control(int sensor, int period, const char *state, app_data_s *ad){

	if (!strcmp(state,RAPROTO_MONITOR_STATE_ON)){
		ad->sensors[sensor].monitor.t = time(NULL);
		ad->sensors[sensor].monitor.period = period;
		ad->sensors[sensor].monitor.on = true;
	} else if (!strcmp(state,RAPROTO_MONITOR_STATE_OFF)){
		ad->sensors[sensor].monitor.on = false;
	} else {
		error_msg(APP_CONTROL_ERROR_INVALID_PARAMETER, __func__, "unknown state");
	}
}


void
monitor_init(app_data_s *ad){
	for (int j = 0; j < RAPROTO_SENSOR_MAX_NUM; j++){
		ad->sensors[j].monitor.on = false;
		ad->sensors[j].monitor.period = 1;
		ad->sensors[j].monitor.t = time(NULL);
	}
}


bool
monitor_cb(app_data_s *ad){
	int j;
	bool logging = false;

	time_t cur_time = time(NULL);

	// check for logging
	for (j = 0; j < RAPROTO_SENSOR_MAX_NUM; j++){
		if (ad->sensors[j].monitor.on) {
			logging = true;
			double diff = difftime(cur_time, ad->sensors[j].monitor.t);
			//dlog_print(DLOG_INFO, LOG_TAG, "index: %d, %f", j, diff);
			if (diff > (ad->sensors[j].monitor.period * 2)) {
				dlog_print(DLOG_ERROR, LOG_TAG, "index: %d, %f", j, diff);
				if ((diff > (ad->sensors[j].monitor.period * 10)) && (diff > RAPROTO_TIMEOUT_MONITOR_SECONDS)) {
					task_error(RAPROTO_ERROR_MSG_MONITOR, ad);
				} else {
					task_warn(RAPROTO_ERROR_MSG_MONITOR, ad);
				}
				return false;
			}
		}
	}

	return logging;
}



