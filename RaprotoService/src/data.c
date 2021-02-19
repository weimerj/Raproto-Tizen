/*
 * data.c
 *
 *  Created on: Feb 14, 2021
 *      Author: weimer
 */


#include "data.h"

void
data_save(bundle *data, const char *filename, app_data_s *ad){
	int err;

	if (data == NULL){
		return;
	}

	bundle_raw *r;
	int len;
	char path[MAX_FILE_PATH];
	sprintf(path,"%s%s\n",app_get_data_path(), filename);
	FILE *fp = fopen(path, "wb");
	if (fp != NULL) {
		if ((err = bundle_encode(data, &r, &len)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "bundle encode");
		if ((err = fwrite(r, 1, len, fp)) != len) error_msg(err, __func__, "fwrite error");
		fclose(fp);
		free(r);
		if  ((len > 40000000) && (ad != NULL))
			ad->scheduler.tasks[RAPROTO_TASK_STOP_SOFT] = RAPROTO_TASK_SCHEDULE_ON;

		dlog_print(DLOG_INFO, LOG_TAG, "Saved Data: %d bytes, %s", len, path);
	} else {
		dlog_print(DLOG_ERROR, LOG_TAG, "fopen fail: %s", path);
	}
}


Eina_Bool
data_autosave(void *data){
	app_data_s* ad = (app_data_s*)data;
	log_message_pack_all(ad,0);
	data_save(ad->data.messages, RAPROTO_DATA_FILENAME, ad);
	return ECORE_CALLBACK_RENEW;
}


void
data_load_user(char *key, app_control_h app, app_data_s *ad) {
	int err;
	char *val;
	//dlog_print(DLOG_INFO, LOG_TAG, "%s", key);
	err = app_control_get_extra_data(app, key, &val);
	if (err == APP_CONTROL_ERROR_NONE) {
		if ((err = bundle_del(ad->settings, key)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "del key");
		if ((err = bundle_add_str(ad->settings, key, val)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "add str");
	} else if (err == APP_CONTROL_ERROR_KEY_NOT_FOUND) {
		return;
	} else {
		error_msg(err, __func__, "get extra data");
	}
}



bundle*
data_load_settings(){
	bundle *settings;
	bundle_raw *r;
	char path[MAX_FILE_PATH];
	int filelen;
	sprintf(path,"%s%s\n",app_get_data_path(),RAPROTO_SETTING_FILENAME);
	FILE *fp = fopen(path, "rb");
	if (fp != NULL){
		fseek(fp,0,SEEK_END);
		filelen = ftell(fp);
		rewind(fp);

		r = (bundle_raw *)malloc(filelen * sizeof(char));
		fread(r, 1, filelen, fp);
		settings = bundle_decode(r, filelen);
		free(r);
		fclose(fp);
	} else {
		settings = settings_init();
		data_save(settings, RAPROTO_SETTING_FILENAME, NULL);
		dlog_print(DLOG_INFO, LOG_TAG, "No Saved Configuration: %s", path);
	}

	return settings;
}





bundle*
data_load_data(){
	bundle *data;
	bundle_raw *r;
	char path[MAX_FILE_PATH];
	int filelen, err;
	sprintf(path,"%s%s\n",app_get_data_path(), RAPROTO_DATA_FILENAME);
	FILE *fp = fopen(path, "rb");
	if (fp != NULL){
		fseek(fp,0,SEEK_END);
		filelen = ftell(fp);
		rewind(fp);

		r = (bundle_raw *)malloc(filelen * sizeof(char));
		if (r == NULL) error_msg(BUNDLE_ERROR_OUT_OF_MEMORY, __func__, "malloc out of memory");
		fread(r, 1, filelen, fp);
		data = bundle_decode(r, filelen);
		if (data == NULL) error_msg(BUNDLE_ERROR_INVALID_PARAMETER, __func__, "bundle decode");
		free(r);
		fclose(fp);
		dlog_print(DLOG_INFO, LOG_TAG, "Loaded %d bytes: %s", filelen, path);
	} else {
		data = bundle_create();
		if (data == NULL) error_msg(BUNDLE_ERROR_OUT_OF_MEMORY, __func__, "bundle create");
		int val = 0;
		if ((err = bundle_add_byte(data, RAPROTO_DATA_IDX_START, &val, sizeof(int))) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "add start idx");
		if ((err = bundle_add_byte(data, RAPROTO_DATA_IDX_END, &val, sizeof(int))) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "add end idx");

		dlog_print(DLOG_INFO, LOG_TAG, "No Saved Data: %s", path);
	}

	return data;
}





void
data_setup(app_data_s *ad){
	int rc;
	int *payloadmax;
	size_t n_size;

	if (!(ad->data.initialized)) {

		ad->data.messages = data_load_data();

		ad->data.mutex = 0;

		if (ad->data.payload != NULL){
			free(ad->data.payload);
			ad->data.payload = NULL;
		}

		if ((rc = bundle_get_byte(ad->settings, RAPROTO_SETTING_MQTT_MAX_PAYLOAD_SIZE, (void**)&payloadmax, &n_size)) != BUNDLE_ERROR_NONE) error_msg(rc, __func__, "payload size");
		ad->data.payload = (char*)calloc(*payloadmax, sizeof(char));
		if (ad->data.payload == NULL) error_msg(BUNDLE_ERROR_OUT_OF_MEMORY, __func__, "calloc out of memory");
		ad->data.payloadmax = *payloadmax;

		ad->data.initialized = true;
	}
}




