/**
 * This file contains the exported symbol.
 */
#include "wifiautoconnect.h"



Eina_Bool
wifi_connect_monitor(void *data){

	wifi_autoconnect_s *ac = (wifi_autoconnect_s*)data;
	int rc;
	int wifi_state;

	if ((rc = wifi_manager_get_connection_state(*(ac->wifi), &wifi_state)) != WIFI_MANAGER_ERROR_NONE)
		if (ac->error_cb != NULL)
			(ac->error_cb)(rc, ac->user_data);

	double diff = difftime(time(NULL),ac->settings.start_time);

	if ((wifi_state == WIFI_MANAGER_CONNECTION_STATE_CONNECTED) || (diff > ac->settings.timeout)){
		if (ac->connect_cb != NULL)
			(ac->connect_cb)(wifi_state, ac->user_data);
		return ECORE_CALLBACK_CANCEL;
	}

	return ECORE_CALLBACK_RENEW;
}




void
wifi_start_connect_timer(wifi_autoconnect_s *ac){

	if (ac->connect_cb == NULL) return;

	ac->settings.start_time = time(NULL);
	ecore_timer_add(WIFI_AUTOCONNECT_PERIOD, wifi_connect_monitor, ac);

	return;
}









bool wifi_manager_found_connect_ap_cb(wifi_manager_ap_h ap, void *data) {

	wifi_autoconnect_s *ac = (wifi_autoconnect_s*)data;


	int rc;
    char *ap_name;
    char *config_name;


    if ((rc = wifi_manager_ap_get_essid(ap, &ap_name)) != WIFI_MANAGER_ERROR_NONE){
    		if (ac->error_cb != NULL)
    			(ac->error_cb)(rc, ac->user_data);
    		return false;
    }


    wifi_autoconnect_config_s *cur_config = ac->wifi_config_head;
    while (cur_config != NULL){
    		if ((rc = wifi_manager_config_get_name(cur_config->config, &config_name)) != WIFI_MANAGER_ERROR_NONE){
    			if (ac->error_cb != NULL)
    				(ac->error_cb)(rc, ac->user_data);
			return false;
		}

		if (strstr(ap_name,config_name)) {
			if ((rc = wifi_manager_connect(*(ac->wifi), ap, NULL, NULL)) != WIFI_MANAGER_ERROR_NONE){
				if (ac->error_cb != NULL)
					(ac->error_cb)(rc, ac->user_data);
				return false;
			}
			return true;
		}

		cur_config = cur_config->next;
    }

    return false;
}



void wifi_manager_scan_cb(wifi_manager_error_e rc, void *data){

	wifi_autoconnect_s *ac = (wifi_autoconnect_s*)data;

	if (rc == WIFI_MANAGER_ERROR_NONE) {
		if ((rc = wifi_manager_foreach_found_ap(*(ac->wifi), wifi_manager_found_connect_ap_cb, ac)) != WIFI_MANAGER_ERROR_NONE)
			if (ac->error_cb != NULL)
				(ac->error_cb)(rc, ac->user_data);
	} else {
		if (ac->error_cb != NULL)
			(ac->error_cb)(rc, ac->user_data);
	}
	return;
}



void wifi_autoconnect_scan(wifi_autoconnect_s *ac){

	int rc;
	wifi_manager_connection_state_e connection_state;


	// Check Wifi connection --> if necessary, connect and/or perform asynchronous publish
	if ((rc = wifi_manager_get_connection_state(*(ac->wifi), &connection_state)) != WIFI_MANAGER_ERROR_NONE){
		if (ac->error_cb != NULL)
			(ac->error_cb)(rc, ac->user_data);
		return;
	}

	switch (connection_state){

		case WIFI_MANAGER_CONNECTION_STATE_DISCONNECTED:
			if ((rc = wifi_manager_scan(*(ac->wifi), wifi_manager_scan_cb, ac)) != WIFI_MANAGER_ERROR_NONE)
				if (ac->error_cb != NULL)
					(ac->error_cb)(rc, ac->user_data);
			break;

		default :
			break;
	}

	return;

}



void wifi_manager_activate_cb(wifi_manager_error_e result, void *data) {

	wifi_autoconnect_s *ac = (wifi_autoconnect_s*)data;

	if (result != WIFI_MANAGER_ERROR_NONE){
		if (ac->error_cb != NULL)
			(ac->error_cb)(result, ac->user_data);
	} else {
		wifi_autoconnect_scan(ac);
	}

	return;

}




void wifi_autoconnect_start(wifi_autoconnect_s *ac, double timeout){

	int rc;
	bool activated = false;
	ac->settings.timeout = timeout;

	wifi_start_connect_timer(ac);

	// Check Wifi activation --> if necessary, activate
	if ((rc = wifi_manager_is_activated(*(ac->wifi), &activated)) != WIFI_MANAGER_ERROR_NONE){
		if (ac->error_cb != NULL)
			(ac->error_cb)(rc, ac->user_data);
		return;
	}

	if (!activated){
		if ((rc = wifi_manager_activate(*(ac->wifi), wifi_manager_activate_cb, ac)) != WIFI_MANAGER_ERROR_NONE)
			if (ac->error_cb != NULL)
				(ac->error_cb)(rc, ac->user_data);
	} else {
		wifi_autoconnect_scan(ac);
	}

	return;
}



bool wifi_manager_config_foreach_configuration_cb(const wifi_manager_config_h config, void *data) {

	int rc;
	wifi_autoconnect_s *ac = (wifi_autoconnect_s*)data;

	wifi_autoconnect_config_s *new_config = (wifi_autoconnect_config_s*)malloc(sizeof(wifi_autoconnect_config_s));
	new_config->next = NULL;

	if ((rc = wifi_manager_config_clone(config,&(new_config->config))) != WIFI_MANAGER_ERROR_NONE){
		free(new_config);
		if (ac->error_cb != NULL)
			(ac->error_cb)(rc, ac->user_data);
	} else {

		if (ac->wifi_config_head == NULL){
			ac->wifi_config_head = new_config;
		} else {

			wifi_autoconnect_config_s *current = ac->wifi_config_head;
			while (current->next != NULL)
				current = current->next;
			current->next = new_config;
		}
	}

	return false;

}



void wifi_autoconnect_create(wifi_autoconnect_s *ac, wifi_manager_h *wifi, wifi_autoconnect_connect_cb connect_cb, wifi_autoconnect_error_cb error_cb, void *user_data){

	wifi_manager_error_e rc = WIFI_MANAGER_ERROR_NONE;

	ac->wifi = wifi;
	ac->wifi_config_head = NULL;
	ac->error_cb = error_cb;
	ac->connect_cb = connect_cb;
	ac->user_data = user_data;

	if ((rc = wifi_manager_config_foreach_configuration(*wifi, wifi_manager_config_foreach_configuration_cb, ac)) != WIFI_MANAGER_ERROR_NONE) {
		if (ac->error_cb != NULL)
			(ac->error_cb)(rc, ac->user_data);
		return;
	}


	return;
}



void wifi_autoconnect_config_destroy(wifi_autoconnect_config_s **wifi_config_head) {

	wifi_autoconnect_config_s *next = *wifi_config_head;

	while (*wifi_config_head != NULL){
		next = (*wifi_config_head)->next;
		wifi_manager_config_destroy((*wifi_config_head)->config);
		free(*wifi_config_head);
		*wifi_config_head = next;
	}

	return;
}



void wifi_autoconnect_destroy(wifi_autoconnect_s *ac) {

	wifi_autoconnect_config_destroy(&(ac->wifi_config_head));

	ac->error_cb = NULL;
	ac->user_data = NULL;
	ac->wifi = NULL;

	return;
}






