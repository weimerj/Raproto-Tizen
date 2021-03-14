/*
 * wifi.c
 *
 *  Created on: Feb 14, 2021
 *      Author: weimer
 */


#include "wifi.h"

static void
wifi_warn(int err, char const *caller_name, char const *id, app_data_s *ad){
	warn_msg(err, caller_name, id);
	task_warn(RAPROTO_ERROR_MSG_WIFI, ad);
	wifi_stop(ad);
}


static void
wifi_connected(wifi_manager_connection_state_e conn_stat, void *data){
	app_data_s *ad = (app_data_s*)data;

	if (conn_stat == WIFI_MANAGER_CONNECTION_STATE_CONNECTED) task_process_cb(RAPROTO_TASK_WIFI_ON_DONE);
	else wifi_warn(WIFI_MANAGER_ERROR_CONNECT_FAILED, __func__, "connection state", ad);

}


void
wifi_initialize(app_data_s *ad) {

	int err;

	// kill the old one
	wifi_deinitialize(ad);

	// create a new manager
	ad->wifi = (wifi_manager_h*)malloc(sizeof(wifi_manager_h));

	// initialize Wifi manager
	if ((err = wifi_manager_initialize(ad->wifi)) != WIFI_MANAGER_ERROR_NONE){
		free(ad->wifi);
		warn_msg(err, __func__, "initialize"); // use warn_msg here ... not wifi_warn
		return;
	}

	// Create Wifi Autoconnect Handler
	wifi_autoconnect_create(&(ad->wifi_ac), ad->wifi, wifi_connected, NULL, ad);  //TODO: add a wifi autoconnect error callback

	return;
}



void
wifi_deinitialize(app_data_s *ad){
	if (ad->wifi != NULL) {
		int err;
		//if ((err = wifi_manager_deinitialize(*(ad->wifi))) != WIFI_MANAGER_ERROR_NONE) error_msg(err, __func__, "deinitialize");
		if ((err = wifi_manager_deinitialize(*(ad->wifi))) != WIFI_MANAGER_ERROR_NONE) warn_msg(err, __func__, "deinitialize"); // use warn_msg here ... not wifi_warn.
		free(ad->wifi);
		ad->wifi = NULL;
	}
}



static void
wifi_deactivate_cb(wifi_manager_error_e err, void *data){
	app_data_s *ad = (app_data_s*)data;
	if (err != WIFI_MANAGER_ERROR_NONE) return wifi_warn(err, __func__, "wifi_deactivate_cb", ad);
	return;
}



static void
wifi_deactivate(app_data_s *ad){
	bool activated;
	wifi_manager_error_e err;

	int *wifi_control;
	size_t wifi_control_size;
	if ((err = bundle_get_byte(ad->settings, RAPROTO_SETTING_WIFI, (void**)&wifi_control, &wifi_control_size)) != BUNDLE_ERROR_NONE) return wifi_warn(err, __func__, "get wifi setting", ad);

	if (*wifi_control > 0) {

		wifi_initialize(ad); // creates wifi and autoconnect

		if (*wifi_control == RAPROTO_WIFI_CONTROL_FULL){
			if ((err = wifi_manager_is_activated(*(ad->wifi),&activated)) != WIFI_MANAGER_ERROR_NONE) return wifi_warn(err, __func__, "wifi_manager_is_activated", ad);

			if (activated){
				if ((err = wifi_manager_deactivate(*(ad->wifi), wifi_deactivate_cb, ad)) != WIFI_MANAGER_ERROR_NONE) return wifi_warn(err, __func__, "wifi_manager_deactivate", ad);
			}
		}
	}
	return;
}



void
wifi_stop(app_data_s *ad){

	int err;
	int *wifi_control;
	size_t wifi_control_size;

	if ((err = bundle_get_byte(ad->settings, RAPROTO_SETTING_WIFI, (void**)&wifi_control, &wifi_control_size)) != BUNDLE_ERROR_NONE){
		warn_msg(err, __func__, "get wifi setting"); // use warn_msg here ... not wifi_warn
		wifi_initialize(ad);
	} else {
		if (*wifi_control == RAPROTO_WIFI_CONTROL_FULL) wifi_deactivate(ad);
		else wifi_initialize(ad);
	}

	task_process_cb(RAPROTO_TASK_WIFI_OFF_DONE);

}




void
wifi_start(app_data_s *ad) {

	int err;
	int *wifi_mode;
	int *wifi_timeout;
	size_t n_size;
	bool active;
	wifi_manager_connection_state_e connection_state;

	if (ad->wifi == NULL) return wifi_warn(WIFI_MANAGER_ERROR_NONE, __func__, "not initialized", ad);

	if ((err = bundle_get_byte(ad->settings, RAPROTO_SETTING_WIFI, (void**)&wifi_mode, &n_size)) != BUNDLE_ERROR_NONE) return wifi_warn(err, __func__, "wifi mode", ad);
	if ((err = bundle_get_byte(ad->settings, RAPROTO_SETTING_WIFI_TIMEOUT, (void**)&wifi_timeout, &n_size)) != BUNDLE_ERROR_NONE) return wifi_warn(err, __func__, "wifi timeout", ad);

	if ((*wifi_mode == RAPROTO_WIFI_CONTROL_ON_ONLY) || (*wifi_mode == RAPROTO_WIFI_CONTROL_FULL)) {
		wifi_autoconnect_start(&(ad->wifi_ac),(double)*wifi_timeout);
	} else if (*wifi_mode == RAPROTO_WIFI_CONTROL_NONE) {
		if ((err = wifi_manager_is_activated(*(ad->wifi), &active)) !=  WIFI_MANAGER_ERROR_NONE) return wifi_warn(err, __func__, "wifi_manager_is_activated", ad);

		if (!active) wifi_connected(WIFI_MANAGER_CONNECTION_STATE_DISCONNECTED, (void*)ad);

		if ((err = wifi_manager_get_connection_state(*(ad->wifi), &connection_state)) != WIFI_MANAGER_ERROR_NONE) return wifi_warn(err, __func__, "wifi_manager_get_connection_state", ad);

		wifi_connected(connection_state, (void*)ad);

	} else {
		return wifi_warn(WIFI_MANAGER_ERROR_OPERATION_ABORTED, __func__, "RAPROTO_WIFI_CONTROL error", ad);
	}

	return;
}



