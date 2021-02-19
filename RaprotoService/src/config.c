/*
 * config.c
 *
 *  Created on: Feb 14, 2021
 *      Author: weimer
 */

#include "jsmn.h"
#include "config.h"


void
config_publish(bundle *settings){
	int err;
	if ((err = event_publish_app_event(RAPROTO_TOPIC_SETTINGS, settings)) != EVENT_ERROR_NONE) error_msg(err,__func__, "publish config");
	//dlog_print(DLOG_INFO, LOG_TAG, "sent configuration / settings");
}


void
config_status(char *val, bundle *settings){
	int err;
	if ((err = bundle_del(settings, RAPROTO_SETTING_CONFIG_STATUS)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "del last update");
	if ((err = bundle_add_str(settings, RAPROTO_SETTING_CONFIG_STATUS, val)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "in progress");
	config_publish(settings);
}



static Raproto_Error_Type_t
config_is_valid_setting(const char *key, jsmntype_t value_type, jsmntok_t *t, char *s) {
	if (!strncmp(&s[t[0].start], key,t[0].end-t[0].start) && (t[0].end-t[0].start == strlen(key)) && (t[1].type == value_type) && (t[0].type == JSMN_STRING))
		return RAPROTO_ERROR_NONE;
	return RAPROTO_ERROR_INVALID_INPUT;
}


static Raproto_Error_Type_t
config_get_setting_for_key(bundle *b, jsmntok_t *t, char *s) {
	// extract key
	char *key;
	key = calloc(t[0].end - t[0].start + 2, sizeof(char));
	if (t[0].type == JSMN_STRING){
		strncpy(key, &s[t[0].start], t[0].end - t[0].start);
	} else {
		dlog_print(DLOG_ERROR, LOG_TAG, "key value invalid: %d", t[0].type);
		return RAPROTO_ERROR_INVALID_INPUT;
	}

	//extract (and store) key value
	if (t[1].type == JSMN_PRIMITIVE) {
		int data = atoi(&s[t[1].start]);
		bundle_del(b, key);
		bundle_add_byte(b, key, &data, sizeof(int));
		//dlog_print(DLOG_INFO, LOG_TAG, "%s : %d",key, data);
	} else if (t[1].type == JSMN_STRING) {
		char *str;
		str = calloc(t[1].end - t[1].start + 1, sizeof(char));
		strncpy(str, &s[t[1].start], t[1].end - t[1].start);
		bundle_del(b, key);
		bundle_add_str(b, key, str);
		//dlog_print(DLOG_INFO, LOG_TAG, "%s : %s",key, str);
		free(str);
	} else {
		dlog_print(DLOG_ERROR, LOG_TAG, "value entry invalid : %d", t[1].type);
		return RAPROTO_ERROR_INVALID_INPUT;
	}

	free(key);
	return RAPROTO_ERROR_NONE;
}



static Raproto_Error_Type_t
config_get_settings_from_json(bundle *settings, char *s){

	int flag = 0;
	int rc = 0;
	int num_entries;
	jsmn_parser p;
	jsmntok_t t[RAPROTO_MAX_NUM_OF_SETTINGS];

	jsmn_init(&p);
	if ((num_entries = jsmn_parse(&p, s, strlen(s), t, RAPROTO_MAX_NUM_OF_SETTINGS)) < 0){
		dlog_print(DLOG_ERROR, LOG_TAG, "JSON parse error: %d", num_entries);
		return RAPROTO_ERROR_INVALID_INPUT;
	}

	//for (int j=0; j < num_entries; j++){
	//	dlog_print(DLOG_ERROR, LOG_TAG, "%s", &s[t[j].start]);
	//}
	//dlog_print(DLOG_INFO, LOG_TAG, "JSON parsed %d items", num_entries);

	for (int n=1; n < num_entries-1; n=n+2){
		//dlog_print(DLOG_INFO, LOG_TAG, "token %d: %d, %d, %d, %d",n, t[n].type, t[n].start, t[n].end, t[n].size);

		if ((rc = config_is_valid_setting(RAPROTO_SETTING_NAME, JSMN_STRING, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_MQTT_PUB_TOPIC, JSMN_STRING, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_MQTT_QOS, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_MQTT_KEEP_ALIVE, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_MQTT_TIMEOUT, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_MQTT_MAX_PAYLOAD_SIZE, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_TX_RATE, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_SAVE_RATE, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_BATTERY, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_POWER, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_WIFI, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_WIFI_TIMEOUT, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_HEART_BEAT, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_SENSOR_ACC, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_SENSOR_HRM, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_SENSOR_PPG, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_SENSOR_TEMP, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_SENSOR_SLEEP, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_SENSOR_STEPS, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_SENSOR_STRESS, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_SENSOR_GYRO, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_SENSOR_MAG, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
		else {
			dlog_print(DLOG_ERROR, LOG_TAG, "unknown key: flag = %d: %s", flag, &s[t[n].start]);
			return RAPROTO_ERROR_INVALID_INPUT;
		}
	}

	return RAPROTO_ERROR_NONE;
}



void
config_received(app_data_s *ad,  char *payload, int payloadLen) {
    char *str;
	int rc = 0;
	int num_entries;
	jsmn_parser p;
	jsmntok_t t[RAPROTO_MAX_NUM_OF_SETTINGS];

	// Use a placeholder for settings -- we don't want to overwrite our local settings (yet)
	bundle *settings;
	settings = bundle_dup(ad->settings);

	jsmn_init(&p);
	if ((num_entries = jsmn_parse(&p, payload, payloadLen, t, RAPROTO_MAX_NUM_OF_SETTINGS)) < 0){
		dlog_print(DLOG_ERROR, LOG_TAG, "JSON parse error here: %d", num_entries);
		task_error(RAPROTO_ERROR_MSG_CONFIG, ad);
		//mqtt_stop(ad);
		return;
	}

	// remove "shared" and parse the rest
	for (int n=0; n<num_entries; n++){
		if (!strncmp(&payload[t[n].start], "shared",t[n].end-t[n].start) && (t[n].end-t[n].start == 6)){
			str = calloc(t[n+1].end-t[n+1].start+1,sizeof(char));
			strncpy(str, &payload[t[n+1].start], t[n+1].end-t[n+1].start);
			dlog_print(DLOG_INFO, LOG_TAG, "Parsing config json: %s", str);
			if ((rc = config_get_settings_from_json(settings, str)) != RAPROTO_ERROR_NONE) {
				dlog_print(DLOG_ERROR, LOG_TAG, "error parsing mqtt shared attributes string");
				task_error(RAPROTO_ERROR_MSG_CONFIG, ad);
				//mqtt_stop(ad);
				return;
			}
			free(str);

			utility_time_update(settings, RAPROTO_SETTING_LAST_UPDATE, "%Y-%m-%d %H:%M:%S");

			break;
		}
	}


	// Store the latest settings
	bundle_free(ad->settings);
	ad->settings = settings;
	data_save(ad->settings, RAPROTO_SETTING_FILENAME, NULL);

	config_publish(ad->settings);
	dlog_print(DLOG_INFO, LOG_TAG, "parsed configuration");

	config_status(RAPROTO_UPDATE_COMPLETE_MESSAGE, ad->settings);
	task_process_cb(RAPROTO_TASK_CONFIG_DONE);

    return;
}





static void
config_request_failure_cb(void* context, MQTTAsync_failureData* response){
	app_data_s *ad = (app_data_s*)context;
	task_error(RAPROTO_ERROR_MSG_MQTT, ad);
	//mqtt_stop(ad);
}

static void
config_request_success_cb(void* context, MQTTAsync_successData* response){
	//app_data_s *ad = (app_data_s*)context;
	return;
}


static void
config_request(app_data_s *ad)
{

	int rc;

	// load settings
	char *sub_topic, *pub_topic, *pub_msg;
	bundle_get_str(ad->settings, RAPROTO_SETTING_MQTT_CONFIG_SUB_TOPIC, &sub_topic);
	bundle_get_str(ad->settings, RAPROTO_SETTING_MQTT_CONFIG_PUB_TOPIC, &pub_topic);
	bundle_get_str(ad->settings, RAPROTO_SETTING_MQTT_CONFIG_PUB_MSG, &pub_msg);

	MQTTAsync_responseOptions subopts = MQTTAsync_responseOptions_initializer;
	subopts.onSuccess = config_request_success_cb;
	subopts.onFailure = config_request_failure_cb; // send on failure for QOS 0
	subopts.context = ad;

    // Subscribe to attribute topic
    if ((rc = MQTTAsync_subscribe(*(ad->mqtt.client), sub_topic, 0, &subopts)) != MQTTASYNC_SUCCESS){
    		task_error(RAPROTO_ERROR_MSG_MQTT, ad);
    		//mqtt_stop(ad);
    		return;
    }


    MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
    pubmsg.payload = pub_msg;
    pubmsg.payloadlen = strlen(pub_msg);
    pubmsg.qos = 0; // only support QOS 0 for configuration (who cares if the packet gets dropped)
    pubmsg.retained = 0;

	MQTTAsync_responseOptions pubopts = MQTTAsync_responseOptions_initializer;
	pubopts.onSuccess = config_request_success_cb;
	pubopts.onFailure = config_request_failure_cb; // send on failure for QOS 0
	pubopts.context = ad;

	MQTTAsync_sendMessage(*(ad->mqtt.client), pub_topic, &pubmsg, &pubopts);

	dlog_print(DLOG_INFO, LOG_TAG, "Configure request sent");

    return;
}


void
config_start(app_data_s *ad){
	config_request(ad);
	return;
}



