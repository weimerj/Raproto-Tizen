/*
 * error.c
 *
 *  Created on: Feb 13, 2021
 *      Author: weimer
 */


#include "error.h"



void
error_msg(int rc, char const *caller_name, char const *id) {
	dlog_print(DLOG_ERROR, LOG_TAG, "%s : %s : [%d, %s]",caller_name, id, rc, get_error_message(rc));
	int err;

	bundle *b = bundle_create();
	if ((err = event_publish_app_event(RAPROTO_ERROR_EVENT, b)) != EVENT_ERROR_NONE) error_msg(err,__func__, "error event");
	bundle_free(b);

	return;
}


void
warn_msg(int rc, char const *caller_name, char const *id) {
	dlog_print(DLOG_WARN, LOG_TAG, "%s : %s : [%d, %s]",caller_name, id, rc, get_error_message(rc));
	return;
}


void
error_msg_event(const char *event_name, bundle *junk, void *data){
	app_data_s *ad = (app_data_s*)data;
	int err;
	int val;

	if ((err = bundle_del(ad->settings, RAPROTO_SETTING_ERROR)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "delete error byte");
	val = RAPROTO_SETTING_ERROR_TRUE;
	if ((err = bundle_add_byte(ad->settings, RAPROTO_SETTING_ERROR, &val, sizeof(int))) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "set error byte");

	service_app_exit();
	return;
}


