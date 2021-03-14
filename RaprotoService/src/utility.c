/*
 * utility.c
 *
 *  Created on: Feb 14, 2021
 *      Author: weimer
 */


#include "utility.h"

void
refresh_display(bundle *settings){
	int err;
	if ((err = event_publish_app_event(RAPROTO_TOPIC_SETTINGS, settings)) != EVENT_ERROR_NONE) error_msg(err,__func__, "publish config");
	//dlog_print(DLOG_INFO, LOG_TAG, "sent configuration / settings");
}

void
utility_log_amount_of_data(app_data_s *ad){
	int *idx_start;
	int *idx_end;
	size_t n_size;
	int err;

	char msg[RAPROTO_MAX_DISPLAY_SIZE];

	if ((err = bundle_get_byte(ad->data.messages, RAPROTO_DATA_IDX_START, (void**)&idx_start, &n_size)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get idx start");
	if ((err = bundle_get_byte(ad->data.messages, RAPROTO_DATA_IDX_END, (void**)&idx_end, &n_size)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get idx end");

	sprintf(msg, "%d messages", queue_size(*idx_start, *idx_end, RAPROTO_DATA_MAX_IDX));

	if ((err = bundle_del(ad->settings, RAPROTO_SETTING_DATA_STORED)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "deleting data stored");
	if ((err = bundle_add_str(ad->settings, RAPROTO_SETTING_DATA_STORED, msg)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "adding payload to bundle");

	ad->display = true;
	//refresh_display(ad->settings);
	//dlog_print(DLOG_INFO, LOG_TAG, "%s", msg);
}


int
queue_size(int start_idx, int end_idx, int queue_size){
	int cur_size;

	if (start_idx > end_idx) cur_size = queue_size - start_idx + end_idx;
	else cur_size = end_idx - start_idx;

	return cur_size;
}


void
increment_queue_idx(int* idx, int N, int queue_size){
	*idx = (*idx + N) % queue_size;
	return;
}




void
utility_time_update(bundle *settings, char *key, const char *format){

	struct timeval tv;
	time_t nowtime;
	struct tm *nowtm;

	//tv = time(NULL);
	gettimeofday(&tv, NULL); //TODO: fix, it is depricated
	nowtime = tv.tv_sec;
	nowtm = localtime(&nowtime);
	char update[RAPROTO_MAX_DISPLAY_SIZE];

	strftime(update, sizeof(update), format, nowtm);
	bundle_del(settings, key);
	bundle_add_str(settings, key, update);
}
