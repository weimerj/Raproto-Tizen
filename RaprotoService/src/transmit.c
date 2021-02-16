/*
 * transmit.c
 *
 *  Created on: Feb 14, 2021
 *      Author: weimer
 */

#include "transmit.h"



void transmit_data_increment(app_data_s *ad) {
	int err;
	char key[RAPROTO_MAX_KEY_SIZE];

	int *temp;
	size_t n_size;
	if ((err = bundle_get_byte(ad->data.messages, RAPROTO_DATA_IDX_START, (void**)&temp, &n_size)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get idx start");

	int idx_start = *temp;
	increment_queue_idx(&idx_start, 1, RAPROTO_DATA_MAX_IDX);
	sprintf(key,"%d",idx_start);

	if ((err = bundle_del(ad->data.messages, key)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "del message");

	if ((err = bundle_del(ad->data.messages, RAPROTO_DATA_IDX_START)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "del idx start");
	if ((err = bundle_add_byte(ad->data.messages, RAPROTO_DATA_IDX_START, &idx_start, sizeof(int))) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "add idx start");
}



void transmit_data_failure_cb(void* context, MQTTAsync_failureData* response){
	app_data_s *ad = (app_data_s*)context;
	int *qos;
	size_t n_size;
	int err;

	if ((err = bundle_get_byte(ad->settings, RAPROTO_SETTING_MQTT_QOS, (void**)&qos, &n_size)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get qos");

	if (*qos == 0){
		transmit_data_increment(ad);
		transmit_data(ad);
	} else {
		task_warn(RAPROTO_ERROR_MSG_MQTT, ad);
		//mqtt_stop(ad);
	}

}

void transmit_data_success_cb(void* context, MQTTAsync_successData* response){
	app_data_s *ad = (app_data_s*)context;
	transmit_data_increment(ad);
	transmit_data(ad);
}

void
transmit_data(app_data_s *ad) {

    int err;

	// load settings
    char key[RAPROTO_MAX_KEY_SIZE];
    char *msg;
    int idx_start, *idx_end, *temp;
	int *qos;
	char *topic;
	size_t n_size;
	if ((err = bundle_get_byte(ad->settings, RAPROTO_SETTING_MQTT_QOS, (void**)&qos, &n_size)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get qos");
	if ((err = bundle_get_str(ad->settings, RAPROTO_SETTING_MQTT_PUB_TOPIC, &topic)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get pub topic");
	if ((err = bundle_get_byte(ad->data.messages, RAPROTO_DATA_IDX_START, (void**)&temp, &n_size)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get idx start");
	if ((err = bundle_get_byte(ad->data.messages, RAPROTO_DATA_IDX_END, (void**)&idx_end, &n_size)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get idx end");

	idx_start = *temp;
	if (idx_start == *idx_end){
		utility_time_update(ad->settings, RAPROTO_SETTING_LAST_SYNC, "%Y-%m-%d %H:%M:%S");
		config_publish(ad->settings);
		task_process_cb(RAPROTO_TASK_TRANSMIT_DONE);
		return;
	}

	increment_queue_idx(&idx_start, 1, RAPROTO_DATA_MAX_IDX);
	sprintf(key,"%d",idx_start);
	if ((err = bundle_get_str(ad->data.messages, key, &msg)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get message");

	MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
	pubmsg.qos = *qos;
	pubmsg.retained = 0;
	pubmsg.payloadlen = strlen(msg);
	pubmsg.payload = msg;

	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	opts.onSuccess = transmit_data_success_cb;
	opts.onFailure = transmit_data_failure_cb;
	opts.context = ad;

	MQTTAsync_sendMessage(*(ad->mqtt.client), topic, &pubmsg, &opts);

	dlog_print(DLOG_INFO, LOG_TAG, "message: idx_start = %d, idx_end = %d, payload len = %d", idx_start, *idx_end, strlen(msg));

	return;

}



void
transmit_data_setup(app_data_s *ad){
	data_setup(ad);
	log_message_pack_all(ad, 0);
	data_save(ad->data.messages, RAPROTO_DATA_FILENAME); // save before trying to transmit (just in case)
	task_process_cb(RAPROTO_TASK_TRANSMIT_SETUP_DONE);
	return;
}



void
transmit_start(app_data_s *ad){
	transmit_data(ad);
	return;
}



Eina_Bool
transmit_autorequest(void *data){
	app_data_s* ad = (app_data_s*)data;

	ad->scheduler.tasks[RAPROTO_TASK_TRANSMIT] = RAPROTO_TASK_SCHEDULE_ON;
	task_start(ad);

	return ECORE_CALLBACK_RENEW;
}






