#include "raprotoservice.h"

//
//typedef struct queue_state{
//	char *name;
//	int idx_start;
//	int idx_end;
//	int len;
//} queue_state_s;
//
//
//typedef struct acc_data{
//	struct timeval t;  // time
//	float x; // x-value
//	float y; // y-value
//	float z; // z-value
//} acc_data_s;
//
//
//struct acc_queue{
//	queue_state_s state;
//	acc_data_s data[QUEUE_SIZE_ACCELEROMETER];
//} acc_queue_s = {{"accelerometer",0,0,QUEUE_SIZE_ACCELEROMETER},};
//
//
//
//typedef struct sys_data{
//	struct timeval t; // time
//	int battery; // battery
//} sys_data_s;
//
//
//struct sys_queue{
//	queue_state_s state;
//	sys_data_s data[QUEUE_SIZE_SYSTEM];
//} sys_queue_s = {{"system info",0,0,QUEUE_SIZE_SYSTEM},};
//
//
//
//typedef struct stored_message_data{
//	bundle *data;
//	queue_state_s state;
//	int enabled;
//} stored_message_data_s;
//
//
//typedef int (*mqtt_payload_create)(void *, char *, int);
//
//
//
//
//typedef struct mqtt_message_data{
//	mqtt_payload_create msg_cb;
//	queue_state_s *state;
//	int idx_start;
//	int idx_end;
//} mqtt_message_data_s;
//
//
//typedef struct mqtt_context{
//	MQTTAsync *client;
//	int cur_sensor;
//	mqtt_message_data_s message_data[RAPROTO_SENSOR_MAX_NUM+1];
//	char *payload;
//	int payloadlen;
//} mqtt_context_s;
//
//
//typedef struct task_scheduler{
//	int tasks[RAPROTO_TASK_NUMBER];
//	int active;
//} task_scheduler_s;
//
//typedef struct message_data{
//	bundle *messages;
//	char *payload;
//	int payloadlen;
//	int payloadmax;
//	int mutex;
//	bool initialized;
//} message_data_s;
//
//typedef struct monitor_data{
//	time_t t;
//	bool on;
//	int period;
//} monitor_data_s;
//
//typedef struct sensor_data{
//	monitor_data_s monitor;
//	sensor_h sensor;
//	sensor_listener_h listener;
//} sensor_data_s;
//
//
//// application data (context) that will be passed to functions when needed
//typedef struct app_data {
//    wifi_manager_h *wifi; // wifi manager
//    wifi_autoconnect_s wifi_ac; // autoconnect configuration
//    mqtt_context_s mqtt; // mqtt data
//    bundle *settings; // service application data
//    message_data_s data; // stored sensor data;
//    task_scheduler_s scheduler; // scheduler for tasks
//    sensor_data_s sensors[RAPROTO_SENSOR_MAX_NUM]; // sensors and monitors
//    bool reset;
//    bool logging;
//} app_data_s;
//
//



//void mqtt_stop(app_data_s *);
//static void task_error(const char *, app_data_s *);
//static void task_warn(const char *, app_data_s *);
////void service_app_terminate(app_data_s *);
////void transmit_data(app_data_s *);
////void transmit_setup(app_data_s *, int);
////void wifi_deinitialize(app_data_s *);
////static int wifi_initialize(app_data_s *);
////int mqtt_error_msg(int, char const *);
//void task_start(app_data_s *);
//static void task_process_cb(const char *);
//static void log_message_pack_all(app_data_s *, int);
////static void config_publish(bundle *);

//
//static void
//error_msg(int rc, char const *caller_name, char const *id) {
//	dlog_print(DLOG_ERROR, LOG_TAG, "%s : %s : [%d, %s]",caller_name, id, rc, get_error_message(rc));
//	int err;
//
//	bundle *b = bundle_create();
//	if ((err = event_publish_app_event(RAPROTO_ERROR_EVENT, b)) != EVENT_ERROR_NONE) error_msg(err,__func__, "error event");
//	bundle_free(b);
//
//	return;
//}
//
//
//static void
//error_msg_event(const char *event_name, bundle *junk, void *data){
//	app_data_s *ad = (app_data_s*)data;
//	int err;
//	int val;
//
//	if ((err = bundle_del(ad->settings, RAPROTO_SETTING_ERROR)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "delete error byte");
//	val = RAPROTO_SETTING_ERROR_TRUE;
//	if ((err = bundle_add_byte(ad->settings, RAPROTO_SETTING_ERROR, &val, sizeof(int))) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "set error byte");
//
//	service_app_exit();
//	return;
//}


//
//static void
//warn_msg(int rc, char const *caller_name, char const *id) {
//	dlog_print(DLOG_WARN, LOG_TAG, "%s : %s : [%d, %s]",caller_name, id, rc, get_error_message(rc));
//	return;
//}


//
//void increment_queue_idx(int* idx, int N, int queue_size){
//	*idx = (*idx + N) % queue_size;
//	return;
//}
//
//
//
//static void
//utility_time_update(bundle *settings, char *key, const char *format){
//
//	struct timeval tv;
//	time_t nowtime;
//	struct tm *nowtm;
//
//	//tv = time(NULL);
//	gettimeofday(&tv, NULL); //TODO: fix, it is depricated
//	nowtime = tv.tv_sec;
//	nowtm = localtime(&nowtime);
//	char update[RAPROTO_MAX_DISPLAY_SIZE];
//
//	strftime(update, sizeof(update), format, nowtm);
//	bundle_del(settings, key);
//	bundle_add_str(settings, key, update);
//}

//
//bool
//monitor_heart_beat(int idx, app_data_s *ad){
//	ad->sensors[idx].monitor.t = time(NULL);
//	return ad->sensors[idx].monitor.on;
//}
//
//
//void
//monitor_control(int sensor, int period, const char *state, app_data_s *ad){
//
//	if (!strcmp(state,RAPROTO_MONITOR_STATE_ON)){
//		ad->sensors[sensor].monitor.t = time(NULL);
//		ad->sensors[sensor].monitor.period = period;
//		ad->sensors[sensor].monitor.on = true;
//	} else if (!strcmp(state,RAPROTO_MONITOR_STATE_OFF)){
//		ad->sensors[sensor].monitor.on = false;
//	} else {
//		error_msg(APP_CONTROL_ERROR_INVALID_PARAMETER, __func__, "unknown state");
//	}
//}
//
//
//void
//monitor_init(app_data_s *ad){
//	for (int j = 0; j < RAPROTO_SENSOR_MAX_NUM; j++){
//		ad->sensors[j].monitor.on = false;
//		ad->sensors[j].monitor.period = 1;
//		ad->sensors[j].monitor.t = time(NULL);
//	}
//}
//
//
//bool
//monitor_cb(app_data_s *ad){
//	int j;
//	bool logging = false;
//
//	time_t cur_time = time(NULL);
//
//	// check for logging
//	for (j = 0; j < RAPROTO_SENSOR_MAX_NUM; j++){
//		if (ad->sensors[j].monitor.on) {
//			logging = true;
//			double diff = difftime(cur_time, ad->sensors[j].monitor.t);
//			//dlog_print(DLOG_INFO, LOG_TAG, "index: %d, %f", j, diff);
//			if (diff > (ad->sensors[j].monitor.period * 2)) {
//				dlog_print(DLOG_ERROR, LOG_TAG, "index: %d, %f", j, diff);
//				if ((diff > (ad->sensors[j].monitor.period * 10)) && (diff > RAPROTO_TIMEOUT_MONITOR_SECONDS)) {
//					task_error(RAPROTO_ERROR_MSG_MONITOR, ad);
//				} else {
//					task_warn(RAPROTO_ERROR_MSG_MONITOR, ad);
//				}
//				return false;
//			}
//		}
//	}
//
//	return logging;
//}
//







//static bundle*
//settings_init(){
//	bundle *settings;
//	settings = bundle_create();
//	char *val_str;
//	char val_str_truncated[RAPROTO_TIZEN_ID_KEY_LENGTH+1];
//	int val;
//
//	// default data that is manually updated
//	system_info_get_platform_string(RAPROTO_TIZEN_ID_KEY, &val_str);
//	strncpy(val_str_truncated,val_str,RAPROTO_TIZEN_ID_KEY_LENGTH);
//	bundle_add_str(settings, RAPROTO_SETTING_DEVICE_ID, val_str_truncated);
//	bundle_add_str(settings, RAPROTO_SETTING_MQTT_USERNAME, val_str_truncated);
//	free(val_str);
//
//	bundle_add_str(settings, RAPROTO_SETTING_MQTT_BROKER, RAPROTO_SETTING_MQTT_BROKER_DEFAULT);
//	bundle_add_str(settings, RAPROTO_SETTING_MQTT_CONFIG_SUB_TOPIC, RAPROTO_SETTING_MQTT_CONFIG_SUB_TOPIC_DEFAULT);
//	bundle_add_str(settings, RAPROTO_SETTING_MQTT_CONFIG_PUB_TOPIC, RAPROTO_SETTING_MQTT_CONFIG_PUB_TOPIC_DEFAULT);
//	bundle_add_str(settings, RAPROTO_SETTING_MQTT_PASSWORD, RAPROTO_SETTING_MQTT_PASSWORD_DEFAULT);
//
//	// default data that is updated through auto-update
//	bundle_add_str(settings, RAPROTO_SETTING_NAME, RAPROTO_SETTING_NAME_DEFAULT);
//	bundle_add_str(settings, RAPROTO_SETTING_MQTT_PUB_TOPIC, RAPROTO_SETTING_MQTT_PUB_TOPIC_DEFAULT);
//	val = RAPROTO_SETTING_POWER_DEFAULT;
//	bundle_add_byte(settings, RAPROTO_SETTING_POWER, &val, sizeof(int));
//	val = RAPROTO_SETTING_WIFI_DEFAULT;
//	bundle_add_byte(settings, RAPROTO_SETTING_WIFI, &val, sizeof(int));
//	val = RAPROTO_SETTING_WIFI_TIMEOUT_DEFAULT;
//	bundle_add_byte(settings, RAPROTO_SETTING_WIFI_TIMEOUT, &val, sizeof(int));
//	val = RAPROTO_SETTING_MQTT_TIMEOUT_DEFAULT;
//	bundle_add_byte(settings, RAPROTO_SETTING_MQTT_TIMEOUT, &val, sizeof(int));
//	val = RAPROTO_SETTING_MQTT_MAX_PAYLOAD_SIZE_DEFAULT;
//	bundle_add_byte(settings, RAPROTO_SETTING_MQTT_MAX_PAYLOAD_SIZE, &val, sizeof(int));
//	val = RAPROTO_SETTING_MQTT_QOS_DEFAULT;
//	bundle_add_byte(settings, RAPROTO_SETTING_MQTT_QOS, &val, sizeof(int));
//	val = RAPROTO_SETTING_MQTT_KEEP_ALIVE_DEFAULT;
//	bundle_add_byte(settings, RAPROTO_SETTING_MQTT_KEEP_ALIVE, &val, sizeof(int));
//	val = RAPROTO_SETTING_TX_RATE_DEFAULT;
//	bundle_add_byte(settings, RAPROTO_SETTING_TX_RATE, &val, sizeof(int));
//	val = RAPROTO_SETTING_BATTERY_DEFAULT;
//	bundle_add_byte(settings, RAPROTO_SETTING_BATTERY, &val, sizeof(int));
//	val = RAPROTO_SETTING_SAVE_RATE_DEFAULT;
//	bundle_add_byte(settings, RAPROTO_SETTING_SAVE_RATE, &val, sizeof(int));
//	val = RAPROTO_SETTING_HEART_BEAT_DEFAULT;
//	bundle_add_byte(settings, RAPROTO_SETTING_HEART_BEAT, &val, sizeof(int));
//	val = RAPROTO_SETTING_ERROR_FALSE;
//	bundle_add_byte(settings, RAPROTO_SETTING_ERROR, &val, sizeof(int));
//	val = RAPROTO_SETTING_LOGGING_FALSE;
//	bundle_add_byte(settings, RAPROTO_SETTING_LOGGING, &val, sizeof(int));
//
//	val = RAPROTO_SETTING_SENSOR_GENERIC_DEFAULT;
//	bundle_add_byte(settings, RAPROTO_SETTING_SENSOR_ACC, &val, sizeof(int));
//	bundle_add_byte(settings, RAPROTO_SETTING_SENSOR_HR, &val, sizeof(int));
//	bundle_add_byte(settings, RAPROTO_SETTING_SENSOR_PPG, &val, sizeof(int));
//	bundle_add_byte(settings, RAPROTO_SETTING_SENSOR_TEMP, &val, sizeof(int));
//	bundle_add_byte(settings, RAPROTO_SETTING_SENSOR_SLEEP, &val, sizeof(int));
//	bundle_add_byte(settings, RAPROTO_SETTING_SENSOR_STEPS, &val, sizeof(int));
//	bundle_add_byte(settings, RAPROTO_SETTING_SENSOR_STRESS, &val, sizeof(int));
//	bundle_add_byte(settings, RAPROTO_SETTING_SENSOR_GYRO, &val, sizeof(int));
//	bundle_add_byte(settings, RAPROTO_SETTING_SENSOR_MAG, &val, sizeof(int));
//
//	// data that is updated by the software (only)
//	bundle_add_str(settings, RAPROTO_SETTING_LAST_UPDATE, RAPROTO_SETTING_LAST_UPDATE_DEFAULT);
//	bundle_add_str(settings, RAPROTO_SETTING_CONFIG_STATUS, RAPROTO_SETTING_CONFIG_STATUS_DEFAULT);
//	bundle_add_str(settings, RAPROTO_SETTING_LAST_SYNC, RAPROTO_SETTING_LAST_SYNC_DEFAULT);
//	bundle_add_str(settings, RAPROTO_SETTING_MQTT_CLIENT_ID, RAPROTO_SETTING_MQTT_CLIENT_ID_DEFAULT);
//
//	char str[1000];
//	sprintf(str,"{\"sharedKeys\":\"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\"}",
//		RAPROTO_SETTING_NAME,
//		RAPROTO_SETTING_POWER,
//		RAPROTO_SETTING_WIFI,
//		RAPROTO_SETTING_WIFI_TIMEOUT,
//		RAPROTO_SETTING_MQTT_PUB_TOPIC,
//		RAPROTO_SETTING_MQTT_QOS,
//		RAPROTO_SETTING_MQTT_KEEP_ALIVE,
//		RAPROTO_SETTING_MQTT_TIMEOUT,
//		RAPROTO_SETTING_MQTT_MAX_PAYLOAD_SIZE,
//		RAPROTO_SETTING_SENSOR_ACC,
//		RAPROTO_SETTING_SENSOR_HR,
//		RAPROTO_SETTING_SENSOR_PPG,
//		RAPROTO_SETTING_SENSOR_TEMP,
//		RAPROTO_SETTING_SENSOR_SLEEP,
//		RAPROTO_SETTING_SENSOR_STEPS,
//		RAPROTO_SETTING_SENSOR_STRESS,
//		RAPROTO_SETTING_SENSOR_GYRO,
//		RAPROTO_SETTING_SENSOR_MAG,
//		RAPROTO_SETTING_TX_RATE,
//		RAPROTO_SETTING_BATTERY,
//		RAPROTO_SETTING_SAVE_RATE,
//		RAPROTO_SETTING_HEART_BEAT);
//	bundle_add_str(settings, RAPROTO_SETTING_MQTT_CONFIG_PUB_MSG, str);
//
//	return settings;
//}


//
//static void
//data_save(bundle *data, const char *filename){
//
//	if (data == NULL){
//		return;
//	}
//
//	bundle_raw *r;
//	int len;
//	char path[MAX_FILE_PATH];
//	sprintf(path,"%s%s\n",app_get_data_path(), filename);
//	FILE *fp = fopen(path, "wb");
//	if (fp != NULL) {
//		bundle_encode(data, &r, &len);
//		fwrite(r, 1, len, fp);
//		fclose(fp);
//		free(r);
//		dlog_print(DLOG_INFO, LOG_TAG, "Saved Data: %d bytes, %s", len, path);
//	} else {
//		dlog_print(DLOG_ERROR, LOG_TAG, "fopen fail: %s", path);
//	}
//}
//
//
//Eina_Bool
//data_autosave(void *data){
//	app_data_s* ad = (app_data_s*)data;
//	log_message_pack_all(ad,0);
//	data_save(ad->data.messages, RAPROTO_DATA_FILENAME);
//	return ECORE_CALLBACK_RENEW;
//}
//
//
//static void
//data_load_user(char *key, app_control_h app, app_data_s *ad) {
//	int err;
//	char *val;
//	//dlog_print(DLOG_INFO, LOG_TAG, "%s", key);
//	err = app_control_get_extra_data(app, key, &val);
//	if (err == APP_CONTROL_ERROR_NONE) {
//		if ((err = bundle_del(ad->settings, key)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "del key");
//		if ((err = bundle_add_str(ad->settings, key, val)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "add str");
//	} else if (err == APP_CONTROL_ERROR_KEY_NOT_FOUND) {
//		return;
//	} else {
//		error_msg(err, __func__, "get extra data");
//	}
//}
//
//
//
//static bundle*
//data_load_settings(){
//	bundle *settings;
//	bundle_raw *r;
//	char path[MAX_FILE_PATH];
//	int filelen;
//	sprintf(path,"%s%s\n",app_get_data_path(),RAPROTO_SETTING_FILENAME);
//	FILE *fp = fopen(path, "rb");
//	if (fp != NULL){
//		fseek(fp,0,SEEK_END);
//		filelen = ftell(fp);
//		rewind(fp);
//
//		r = (bundle_raw *)malloc(filelen * sizeof(char));
//		fread(r, 1, filelen, fp);
//		settings = bundle_decode(r, filelen);
//		free(r);
//		fclose(fp);
//	} else {
//		settings = settings_init();
//		data_save(settings, RAPROTO_SETTING_FILENAME);
//		dlog_print(DLOG_INFO, LOG_TAG, "No Saved Configuration: %s", path);
//	}
//
//	return settings;
//}
//
//
//
//
//
//static bundle*
//data_load_data(){
//	bundle *data;
//	bundle_raw *r;
//	char path[MAX_FILE_PATH];
//	int filelen, err;
//	sprintf(path,"%s%s\n",app_get_data_path(), RAPROTO_DATA_FILENAME);
//	FILE *fp = fopen(path, "rb");
//	if (fp != NULL){
//		fseek(fp,0,SEEK_END);
//		filelen = ftell(fp);
//		rewind(fp);
//
//		r = (bundle_raw *)malloc(filelen * sizeof(char));
//		if (r == NULL) error_msg(BUNDLE_ERROR_OUT_OF_MEMORY, __func__, "malloc out of memory");
//		fread(r, 1, filelen, fp);
//		data = bundle_decode(r, filelen);
//		if (data == NULL) error_msg(BUNDLE_ERROR_INVALID_PARAMETER, __func__, "bundle decode");
//		free(r);
//		fclose(fp);
//		dlog_print(DLOG_INFO, LOG_TAG, "Loaded %d bytes: %s", filelen, path);
//	} else {
//		data = bundle_create();
//		if (data == NULL) error_msg(BUNDLE_ERROR_OUT_OF_MEMORY, __func__, "bundle create");
//		int val = 0;
//		if ((err = bundle_add_byte(data, RAPROTO_DATA_IDX_START, &val, sizeof(int))) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "add start idx");
//		if ((err = bundle_add_byte(data, RAPROTO_DATA_IDX_END, &val, sizeof(int))) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "add end idx");
//
//		dlog_print(DLOG_INFO, LOG_TAG, "No Saved Data: %s", path);
//	}
//
//	return data;
//}
//
//
//
//
//
//static void
//data_setup(app_data_s *ad){
//	int rc;
//	int *payloadmax;
//	size_t n_size;
//
//	if (!(ad->data.initialized)) {
//
//		ad->data.messages = data_load_data();
//
//		ad->data.mutex = 0;
//
//		if (ad->data.payload != NULL){
//			free(ad->data.payload);
//			ad->data.payload = NULL;
//		}
//
//		if ((rc = bundle_get_byte(ad->settings, RAPROTO_SETTING_MQTT_MAX_PAYLOAD_SIZE, (void**)&payloadmax, &n_size)) != BUNDLE_ERROR_NONE) error_msg(rc, __func__, "payload size");
//		ad->data.payload = (char*)calloc(*payloadmax, sizeof(char));
//		if (ad->data.payload == NULL) error_msg(BUNDLE_ERROR_OUT_OF_MEMORY, __func__, "calloc out of memory");
//		ad->data.payloadmax = *payloadmax;
//
//		ad->data.initialized = true;
//	}
//}
//




//
//static void
//config_status(char *val, bundle *settings){
//	int err;
//	if ((err = bundle_del(settings, RAPROTO_SETTING_CONFIG_STATUS)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "del last update");
//	if ((err = bundle_add_str(settings, RAPROTO_SETTING_CONFIG_STATUS, val)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "in progress");
//	config_publish(settings);
//}
//
//
//static void
//config_publish(bundle *settings){
//	int err;
//	if ((err = event_publish_app_event(RAPROTO_TOPIC_SETTINGS, settings)) != EVENT_ERROR_NONE) error_msg(err,__func__, "publish config");
//	//dlog_print(DLOG_INFO, LOG_TAG, "sent configuration / settings");
//}
//
//
//
//
//static Raproto_Error_Type_t
//config_is_valid_setting(const char *key, jsmntype_t value_type, jsmntok_t *t, char *s) {
//	if (!strncmp(&s[t[0].start], key,t[0].end-t[0].start) && (t[0].end-t[0].start == strlen(key)) && (t[1].type == value_type) && (t[0].type == JSMN_STRING))
//		return RAPROTO_ERROR_NONE;
//	return RAPROTO_ERROR_INVALID_INPUT;
//}
//
//
//static Raproto_Error_Type_t
//config_get_setting_for_key(bundle *b, jsmntok_t *t, char *s) {
//	// extract key
//	char *key;
//	key = calloc(t[0].end - t[0].start + 2, sizeof(char));
//	if (t[0].type == JSMN_STRING){
//		strncpy(key, &s[t[0].start], t[0].end - t[0].start);
//	} else {
//		dlog_print(DLOG_ERROR, LOG_TAG, "key value invalid: %d", t[0].type);
//		return RAPROTO_ERROR_INVALID_INPUT;
//	}
//
//	//extract (and store) key value
//	if (t[1].type == JSMN_PRIMITIVE) {
//		int data = atoi(&s[t[1].start]);
//		bundle_del(b, key);
//		bundle_add_byte(b, key, &data, sizeof(int));
//		//dlog_print(DLOG_INFO, LOG_TAG, "%s : %d",key, data);
//	} else if (t[1].type == JSMN_STRING) {
//		char *str;
//		str = calloc(t[1].end - t[1].start + 1, sizeof(char));
//		strncpy(str, &s[t[1].start], t[1].end - t[1].start);
//		bundle_del(b, key);
//		bundle_add_str(b, key, str);
//		//dlog_print(DLOG_INFO, LOG_TAG, "%s : %s",key, str);
//		free(str);
//	} else {
//		dlog_print(DLOG_ERROR, LOG_TAG, "value entry invalid : %d", t[1].type);
//		return RAPROTO_ERROR_INVALID_INPUT;
//	}
//
//	free(key);
//	return RAPROTO_ERROR_NONE;
//}
//
//
//
//static Raproto_Error_Type_t
//config_get_settings_from_json(bundle *settings, char *s){
//
//	int flag = 0;
//	int rc = 0;
//	int num_entries;
//	jsmn_parser p;
//	jsmntok_t t[RAPROTO_MAX_NUM_OF_SETTINGS];
//
//	jsmn_init(&p);
//	if ((num_entries = jsmn_parse(&p, s, strlen(s), t, RAPROTO_MAX_NUM_OF_SETTINGS)) < 0){
//		dlog_print(DLOG_ERROR, LOG_TAG, "JSON parse error: %d", num_entries);
//		return RAPROTO_ERROR_INVALID_INPUT;
//	}
//
//	//for (int j=0; j < num_entries; j++){
//	//	dlog_print(DLOG_ERROR, LOG_TAG, "%s", &s[t[j].start]);
//	//}
//	//dlog_print(DLOG_INFO, LOG_TAG, "JSON parsed %d items", num_entries);
//
//	for (int n=1; n < num_entries-1; n=n+2){
//		//dlog_print(DLOG_INFO, LOG_TAG, "token %d: %d, %d, %d, %d",n, t[n].type, t[n].start, t[n].end, t[n].size);
//
//		if ((rc = config_is_valid_setting(RAPROTO_SETTING_NAME, JSMN_STRING, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
//		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_MQTT_PUB_TOPIC, JSMN_STRING, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
//		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_MQTT_QOS, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
//		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_MQTT_KEEP_ALIVE, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
//		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_MQTT_TIMEOUT, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
//		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_MQTT_MAX_PAYLOAD_SIZE, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
//		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_TX_RATE, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
//		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_SAVE_RATE, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
//		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_BATTERY, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
//		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_POWER, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
//		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_WIFI, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
//		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_WIFI_TIMEOUT, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
//		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_HEART_BEAT, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
//		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_SENSOR_ACC, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
//		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_SENSOR_HR, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
//		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_SENSOR_PPG, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
//		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_SENSOR_TEMP, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
//		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_SENSOR_SLEEP, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
//		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_SENSOR_STEPS, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
//		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_SENSOR_STRESS, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
//		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_SENSOR_GYRO, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
//		else if ((rc = config_is_valid_setting(RAPROTO_SETTING_SENSOR_MAG, JSMN_PRIMITIVE, &t[n], s)) == RAPROTO_ERROR_NONE) config_get_setting_for_key(settings, &t[n], s);
//		else {
//			dlog_print(DLOG_ERROR, LOG_TAG, "unknown key: flag = %d: %s", flag, &s[t[n].start]);
//			return RAPROTO_ERROR_INVALID_INPUT;
//		}
//	}
//
//	return RAPROTO_ERROR_NONE;
//}
//
//
//
//void config_received(app_data_s *ad,  char *payload, int payloadLen)
//{
//    char *str;
//	int rc = 0;
//	int num_entries;
//	jsmn_parser p;
//	jsmntok_t t[RAPROTO_MAX_NUM_OF_SETTINGS];
//
//	// Use a placeholder for settings -- we don't want to overwrite our local settings (yet)
//	bundle *settings;
//	settings = bundle_dup(ad->settings);
//
//	jsmn_init(&p);
//	if ((num_entries = jsmn_parse(&p, payload, payloadLen, t, RAPROTO_MAX_NUM_OF_SETTINGS)) < 0){
//		dlog_print(DLOG_ERROR, LOG_TAG, "JSON parse error here: %d", num_entries);
//		task_error(RAPROTO_ERROR_MSG_CONFIG, ad);
//		//mqtt_stop(ad);
//		return;
//	}
//
//	// remove "shared" and parse the rest
//	for (int n=0; n<num_entries; n++){
//		if (!strncmp(&payload[t[n].start], "shared",t[n].end-t[n].start) && (t[n].end-t[n].start == 6)){
//			str = calloc(t[n+1].end-t[n+1].start+1,sizeof(char));
//			strncpy(str, &payload[t[n+1].start], t[n+1].end-t[n+1].start);
//			dlog_print(DLOG_INFO, LOG_TAG, "Parsing config json: %s", str);
//			if ((rc = config_get_settings_from_json(settings, str)) != RAPROTO_ERROR_NONE) {
//				dlog_print(DLOG_ERROR, LOG_TAG, "error parsing mqtt shared attributes string");
//				task_error(RAPROTO_ERROR_MSG_CONFIG, ad);
//				//mqtt_stop(ad);
//				return;
//			}
//			free(str);
//
//			utility_time_update(settings, RAPROTO_SETTING_LAST_UPDATE, "%Y-%m-%d %H:%M:%S");
//
//			break;
//		}
//	}
//
//
//	// Store the latest settings
//	bundle_free(ad->settings);
//	ad->settings = settings;
//	data_save(ad->settings, RAPROTO_SETTING_FILENAME);
//
//	config_publish(ad->settings);
//	dlog_print(DLOG_INFO, LOG_TAG, "parsed configuration");
//
//	config_status(RAPROTO_UPDATE_COMPLETE_MESSAGE, ad->settings);
//	task_process_cb(RAPROTO_TASK_CONFIG_DONE);
//
//
//    return;
//}
//
//
//
//
//
//static void
//config_request_failure_cb(void* context, MQTTAsync_failureData* response){
//	app_data_s *ad = (app_data_s*)context;
//	task_error(RAPROTO_ERROR_MSG_MQTT, ad);
//	//mqtt_stop(ad);
//}
//
//static void
//config_request_success_cb(void* context, MQTTAsync_successData* response){
//	//app_data_s *ad = (app_data_s*)context;
//	return;
//}
//
//
//static void
//config_request(app_data_s *ad)
//{
//
//	int rc;
//
//	// load settings
//	char *sub_topic, *pub_topic, *pub_msg;
//	bundle_get_str(ad->settings, RAPROTO_SETTING_MQTT_CONFIG_SUB_TOPIC, &sub_topic);
//	bundle_get_str(ad->settings, RAPROTO_SETTING_MQTT_CONFIG_PUB_TOPIC, &pub_topic);
//	bundle_get_str(ad->settings, RAPROTO_SETTING_MQTT_CONFIG_PUB_MSG, &pub_msg);
//
//	MQTTAsync_responseOptions subopts = MQTTAsync_responseOptions_initializer;
//	subopts.onSuccess = config_request_success_cb;
//	subopts.onFailure = config_request_failure_cb; // send on failure for QOS 0
//	subopts.context = ad;
//
//    // Subscribe to attribute topic
//    if ((rc = MQTTAsync_subscribe(*(ad->mqtt.client), sub_topic, 0, &subopts)) != MQTTASYNC_SUCCESS){
//    		task_error(RAPROTO_ERROR_MSG_MQTT, ad);
//    		//mqtt_stop(ad);
//    		return;
//    }
//
//
//    MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
//    pubmsg.payload = pub_msg;
//    pubmsg.payloadlen = strlen(pub_msg);
//    pubmsg.qos = 0; // only support QOS 0 for configuration (who cares if the packet gets dropped)
//    pubmsg.retained = 0;
//
//	MQTTAsync_responseOptions pubopts = MQTTAsync_responseOptions_initializer;
//	pubopts.onSuccess = config_request_success_cb;
//	pubopts.onFailure = config_request_failure_cb; // send on failure for QOS 0
//	pubopts.context = ad;
//
//	MQTTAsync_sendMessage(*(ad->mqtt.client), pub_topic, &pubmsg, &pubopts);
//
//	dlog_print(DLOG_INFO, LOG_TAG, "Configure request sent");
//
//    return;
//}
//
//
//static void
//config_start(app_data_s *ad){
//	config_request(ad);
//	return;
//}
//
//
//


//
//
//void
//mqtt_destroy(app_data_s *ad){
//
//	if (ad->mqtt.client != NULL) {
//		MQTTAsync_destroy(ad->mqtt.client);
//		free(ad->mqtt.client);
//		ad->mqtt.client = NULL;
//	}
//
//	task_process_cb(RAPROTO_TASK_MQTT_DESTROY_DONE);
//}
//
//
//
//static void
//mqtt_stop_failure_cb(void* context, MQTTAsync_failureData* response){
//	app_data_s *ad = (app_data_s*)context;
//	task_error(RAPROTO_ERROR_MSG_MQTT, ad);
//	task_process_cb(RAPROTO_TASK_MQTT_OFF_DONE);
//}
//
//
//static void
//mqtt_stop_success_cb(void* context, MQTTAsync_successData* response){
//	//app_data_s *ad = (app_data_s*)context;
//	task_process_cb(RAPROTO_TASK_MQTT_OFF_DONE);
//}
//
//
//void
//mqtt_stop(app_data_s *ad){
//	int err;
//
//	MQTTAsync_disconnectOptions opts = MQTTAsync_disconnectOptions_initializer;
//	opts.onSuccess = mqtt_stop_success_cb;
//	opts.onFailure = mqtt_stop_failure_cb;
//	opts.timeout = 10000;
//	opts.context = ad;
//
//	if ((err = MQTTAsync_disconnect(*(ad->mqtt.client), &opts)) != MQTTASYNC_SUCCESS) task_error(RAPROTO_ERROR_MSG_MQTT,ad);
//
//	return;
//}
//
//
//
//static int
//mqtt_message_received(void *context, char *topicName, int topicLen, MQTTAsync_message *message)
//{
//	app_data_s *ad = (app_data_s*)context;
//	char *payload = message->payload;
//	int err;
//
//	dlog_print(DLOG_INFO, LOG_TAG, "%s : %d, %s", topicName, message->payloadlen, payload);
//
//	char *config;
//	if ((err = bundle_get_str(ad->settings, RAPROTO_SETTING_MQTT_CONFIG_SUB_TOPIC, &config)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get string");
//
//	if (!strncmp(topicName, config, topicLen-1)) {
//		config_received(ad, message->payload, message->payloadlen);
//	}
//
//	return 1;
//}
//
//
//static void
//mqtt_delivered(void *context, MQTTAsync_token dt)
//{
//	//dlog_print(DLOG_INFO, LOG_TAG, "Message delivered! %d",dt);
//	return;
//}
//
//
//
//static void
//mqtt_connection_lost(void *context, char *cause) {
//	app_data_s *ad = (app_data_s*)context;
//	task_warn(RAPROTO_ERROR_MSG_MQTT, ad);
//	mqtt_stop(ad);
//}
//
//
//
//
//static void
//mqtt_start_success_cb(void* context, MQTTAsync_successData* response){
//	task_process_cb(RAPROTO_TASK_MQTT_ON_DONE);
//	return;
//}
//
//
//
//static void
//mqtt_start_failure_cb(void* context, MQTTAsync_failureData* response){
//	app_data_s *ad = (app_data_s*)context;
//	task_warn(RAPROTO_ERROR_MSG_MQTT, ad);
//	//mqtt_stop(ad);
//	return;
//}
//
//
//
//void
//mqtt_start(app_data_s *ad){
//	int err;
//	wifi_deinitialize(ad);
//
//	if (ad->mqtt.client != NULL){
//		dlog_print(DLOG_ERROR, LOG_TAG, "MEMORY LEAK: mqtt client not null (mqtt_start)");
//	}
//
//	// create a new manager
//	ad->mqtt.client = (MQTTAsync*)malloc(sizeof(MQTTAsync));
//
//	// load settings
//	char *broker, *client_id, *username, *password;
//	int *keep_alive;
//	size_t n_size;
//	bundle_get_str(ad->settings, RAPROTO_SETTING_MQTT_BROKER, &broker);
//	bundle_get_str(ad->settings, RAPROTO_SETTING_MQTT_CLIENT_ID, &client_id);
//	bundle_get_str(ad->settings, RAPROTO_SETTING_MQTT_USERNAME, &username);
//	bundle_get_str(ad->settings, RAPROTO_SETTING_MQTT_PASSWORD, &password);
//	bundle_get_byte(ad->settings, RAPROTO_SETTING_MQTT_KEEP_ALIVE, (void**)&keep_alive, &n_size);
//
//	if ((err = MQTTAsync_create(ad->mqtt.client, broker, client_id, MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTASYNC_SUCCESS){
//		task_error(RAPROTO_ERROR_MSG_MQTT, ad);
//		//mqtt_stop(ad);
//		return;
//	}
//
//    if ((err = MQTTAsync_setCallbacks(*(ad->mqtt.client), ad, mqtt_connection_lost, mqtt_message_received, mqtt_delivered)) != MQTTASYNC_SUCCESS){
//		task_error(RAPROTO_ERROR_MSG_MQTT, ad);
//		//mqtt_stop(ad);
//    		return;
//    }
//
//	MQTTAsync_connectOptions opts = MQTTAsync_connectOptions_initializer;
//	opts.keepAliveInterval = *keep_alive;
//	opts.cleansession = 0;
//	opts.username = username;
//	opts.password = password;
//	opts.connectTimeout = 10;
//	opts.retryInterval = 0;
//	opts.onSuccess = mqtt_start_success_cb;
//	opts.onFailure = mqtt_start_failure_cb;
//	opts.context = ad;
//
//	MQTTAsync_SSLOptions sslopts = MQTTAsync_SSLOptions_initializer;
//	opts.ssl = &sslopts;
//	opts.ssl->enableServerCertAuth = 0;
//
//
//	if ((err = MQTTAsync_connect(*(ad->mqtt.client), &opts)) != MQTTASYNC_SUCCESS){
//		task_warn(RAPROTO_ERROR_MSG_MQTT, ad);
//		//mqtt_stop(ad);
//		return;
//	}
//
//	return;
//}
//
//

//
//
//
//void wifi_deinitialize(app_data_s *ad){
//	if (ad->wifi != NULL) {
//		int err;
//		//if ((err = wifi_manager_deinitialize(*(ad->wifi))) != WIFI_MANAGER_ERROR_NONE) error_msg(err, __func__, "deinitialize");
//		if ((err = wifi_manager_deinitialize(*(ad->wifi))) != WIFI_MANAGER_ERROR_NONE) warn_msg(err, __func__, "deinitialize");
//		free(ad->wifi);
//		ad->wifi = NULL;
//	}
//}
//
//
//
//static void
//wifi_deactivate_cb(wifi_manager_error_e err, void *data){
//	//app_data_s *ad = (app_data_s*)data;
//	if (err != WIFI_MANAGER_ERROR_NONE) error_msg(err, __func__, NULL);
//	return;
//}
//
//
//
//static int
//wifi_deactivate(app_data_s *ad){
//	bool activated;
//	wifi_manager_error_e err;
//
//	int *wifi_control;
//	size_t wifi_control_size;
//	bundle_get_byte(ad->settings, RAPROTO_SETTING_WIFI, (void**)&wifi_control, &wifi_control_size);
//	if (*wifi_control > 0) {
//
//		wifi_initialize(ad); // creates wifi and autoconnect
//
//		if (*wifi_control == RAPROTO_WIFI_CONTROL_FULL){
//			if ((err = wifi_manager_is_activated(*(ad->wifi),&activated)) != WIFI_MANAGER_ERROR_NONE){
//				error_msg(err,__func__, "manager is activated");
//				return err;
//			}
//
//			if (activated){
//				if ((err = wifi_manager_deactivate(*(ad->wifi), wifi_deactivate_cb, ad)) != WIFI_MANAGER_ERROR_NONE) {
//					error_msg(err,__func__, "manager deactivate");
//					return err;
//				}
//			}
//		}
//	}
//	return WIFI_MANAGER_ERROR_NONE;
//}
//
//
//
//void
//wifi_stop(app_data_s *ad){
//
//	int err;
//	int *wifi_control;
//	size_t wifi_control_size;
//	if ((err = bundle_get_byte(ad->settings, RAPROTO_SETTING_WIFI, (void**)&wifi_control, &wifi_control_size)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get wifi setting");
//
//	if (*wifi_control == RAPROTO_WIFI_CONTROL_FULL) {
//		if ((err = wifi_deactivate(ad)) != WIFI_MANAGER_ERROR_NONE) error_msg(err,__func__, "wifi deactivate");
//	} else {
//		if ((err = wifi_initialize(ad)) != WIFI_MANAGER_ERROR_NONE) error_msg(err,__func__, "wifi initialize");
//	}
//
//	task_process_cb(RAPROTO_TASK_WIFI_OFF_DONE);
//
//}
//
//
//void
//wifi_connected(wifi_manager_connection_state_e conn_stat, void *data){
//	app_data_s *ad = (app_data_s*)data;
//
//	if (conn_stat == WIFI_MANAGER_CONNECTION_STATE_CONNECTED) task_process_cb(RAPROTO_TASK_WIFI_ON_DONE);
//	else task_warn(RAPROTO_ERROR_MSG_WIFI, ad);
//
//}
//
//
//
//void
//wifi_start(app_data_s *ad) {
//
//	int err;
//	int *wifi_mode;
//	int *wifi_timeout;
//	size_t n_size;
//
//	if ((err = bundle_get_byte(ad->settings, RAPROTO_SETTING_WIFI, (void**)&wifi_mode, &n_size)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "wifi mode");
//	if ((err = bundle_get_byte(ad->settings, RAPROTO_SETTING_WIFI_TIMEOUT, (void**)&wifi_timeout, &n_size)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "wifi timeout");
//
//
//	if ((*wifi_mode == RAPROTO_WIFI_CONTROL_ON_ONLY) || (*wifi_mode == RAPROTO_WIFI_CONTROL_FULL))
//		wifi_autoconnect_start(&(ad->wifi_ac),(double)*wifi_timeout);
//
//	return;
//}
//
//
//static int
//wifi_initialize(app_data_s *ad) {
//
//	int err;
//
//	// kill the old one
//	wifi_deinitialize(ad);
//
//	// create a new manager
//	ad->wifi = (wifi_manager_h*)malloc(sizeof(wifi_manager_h));
//
//	// initialize Wifi manager
//	if ((err = wifi_manager_initialize(ad->wifi)) != WIFI_MANAGER_ERROR_NONE){
//		free(ad->wifi);
//		error_msg(err, __func__, "initialize");
//		return err;
//	}
//
//	// Create Wifi Autoconnect Handler
//	wifi_autoconnect_create(&(ad->wifi_ac), ad->wifi, wifi_connected, NULL, ad);  //TODO: add a wifi autoconnect error callback
//
//	return WIFI_MANAGER_ERROR_NONE;
//}
//

//
//
//void transmit_data_increment(app_data_s *ad) {
//	int err;
//	char key[RAPROTO_MAX_KEY_SIZE];
//
//	int *temp;
//	size_t n_size;
//	if ((err = bundle_get_byte(ad->data.messages, RAPROTO_DATA_IDX_START, (void**)&temp, &n_size)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get idx start");
//
//	int idx_start = *temp;
//	increment_queue_idx(&idx_start, 1, RAPROTO_DATA_MAX_IDX);
//	sprintf(key,"%d",idx_start);
//
//	if ((err = bundle_del(ad->data.messages, key)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "del message");
//
//	if ((err = bundle_del(ad->data.messages, RAPROTO_DATA_IDX_START)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "del idx start");
//	if ((err = bundle_add_byte(ad->data.messages, RAPROTO_DATA_IDX_START, &idx_start, sizeof(int))) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "add idx start");
//}
//
//
//
//void transmit_data_failure_cb(void* context, MQTTAsync_failureData* response){
//	app_data_s *ad = (app_data_s*)context;
//	int *qos;
//	size_t n_size;
//	int err;
//
//	if ((err = bundle_get_byte(ad->settings, RAPROTO_SETTING_MQTT_QOS, (void**)&qos, &n_size)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get qos");
//
//	if (*qos == 0){
//		transmit_data_increment(ad);
//		transmit_data(ad);
//	} else {
//		task_warn(RAPROTO_ERROR_MSG_MQTT, ad);
//		//mqtt_stop(ad);
//	}
//
//}
//
//void transmit_data_success_cb(void* context, MQTTAsync_successData* response){
//	app_data_s *ad = (app_data_s*)context;
//	transmit_data_increment(ad);
//	transmit_data(ad);
//}
//
//void
//transmit_data(app_data_s *ad) {
//
//    int err;
//
//	// load settings
//    char key[RAPROTO_MAX_KEY_SIZE];
//    char *msg;
//    int idx_start, *idx_end, *temp;
//	int *qos;
//	char *topic;
//	size_t n_size;
//	if ((err = bundle_get_byte(ad->settings, RAPROTO_SETTING_MQTT_QOS, (void**)&qos, &n_size)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get qos");
//	if ((err = bundle_get_str(ad->settings, RAPROTO_SETTING_MQTT_PUB_TOPIC, &topic)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get pub topic");
//	if ((err = bundle_get_byte(ad->data.messages, RAPROTO_DATA_IDX_START, (void**)&temp, &n_size)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get idx start");
//	if ((err = bundle_get_byte(ad->data.messages, RAPROTO_DATA_IDX_END, (void**)&idx_end, &n_size)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get idx end");
//
//	idx_start = *temp;
//	if (idx_start == *idx_end){
//		utility_time_update(ad->settings, RAPROTO_SETTING_LAST_SYNC, "%Y-%m-%d %H:%M:%S");
//		config_publish(ad->settings);
//		task_process_cb(RAPROTO_TASK_TRANSMIT_DONE);
//		return;
//	}
//
//	increment_queue_idx(&idx_start, 1, RAPROTO_DATA_MAX_IDX);
//	sprintf(key,"%d",idx_start);
//	if ((err = bundle_get_str(ad->data.messages, key, &msg)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get message");
//
//	MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
//	pubmsg.qos = *qos;
//	pubmsg.retained = 0;
//	pubmsg.payloadlen = strlen(msg);
//	pubmsg.payload = msg;
//
//	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
//	opts.onSuccess = transmit_data_success_cb;
//	opts.onFailure = transmit_data_failure_cb;
//	opts.context = ad;
//
//	MQTTAsync_sendMessage(*(ad->mqtt.client), topic, &pubmsg, &opts);
//
//	dlog_print(DLOG_INFO, LOG_TAG, "message: idx_start = %d, idx_end = %d, payload len = %d", idx_start, *idx_end, strlen(msg));
//
//	return;
//
//}
//
//
//
//static void
//transmit_data_setup(app_data_s *ad){
//	data_setup(ad);
//	log_message_pack_all(ad, 0);
//	data_save(ad->data.messages, RAPROTO_DATA_FILENAME); // save before trying to transmit (just in case)
//	task_process_cb(RAPROTO_TASK_TRANSMIT_SETUP_DONE);
//	return;
//}
//
//
//
//static void
//transmit_start(app_data_s *ad){
//	transmit_data(ad);
//	return;
//}
//
//
//
//Eina_Bool
//transmit_autorequest(void *data){
//	app_data_s* ad = (app_data_s*)data;
//
//	ad->scheduler.tasks[RAPROTO_TASK_TRANSMIT] = RAPROTO_TASK_SCHEDULE_ON;
//	task_start(ad);
//
//	return ECORE_CALLBACK_RENEW;
//}
//
//
//

//
//
//
//
//static void
//log_message_pack_all(app_data_s *ad, int attempt) {
//	// load settings
//	int err;
//	char key[RAPROTO_MAX_KEY_SIZE];
//
//
//	// implement mutex
//	while(ad->data.mutex != 0) {
//		usleep(RAPROTO_MUTEX_SLEEP);
//	}
//	ad->data.mutex++;
//	if (ad->data.mutex != 1){
//		warn_msg(APP_CONTROL_ERROR_IO_ERROR, __func__, "mutex collision");
//		ad->data.mutex--;
//		attempt++;
//		if (attempt <= RAPROTO_MUTEX_ATTEMPTS) log_message_pack_all(ad, attempt);
//		return;
//	}
//
//	if (ad->data.payload != NULL){
//		if (strlen(ad->data.payload) > 0) {
//
//			int *temp;
//			size_t n_size;
//			if ((err = bundle_get_byte(ad->data.messages, RAPROTO_DATA_IDX_END, (void**)&temp, &n_size)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "retrieving idx end");
//			int idx_end = *temp;
//
//			increment_queue_idx(&idx_end, 1, RAPROTO_DATA_MAX_IDX);
//			if ((err = bundle_del(ad->data.messages, RAPROTO_DATA_IDX_END)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "deleting idx end");
//			if ((err = bundle_add_byte(ad->data.messages, RAPROTO_DATA_IDX_END, &idx_end, sizeof(int))) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "updating idx end");
//
//			sprintf(key, "%d", idx_end);
//			if ((err = bundle_add_str(ad->data.messages, key, ad->data.payload)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "adding payload to bundle");
//
//			dlog_print(DLOG_INFO, LOG_TAG, "message %s: len = %d ", key, strlen(ad->data.payload));
//
//			memset(ad->data.payload, 0, (ad->data.payloadmax) * sizeof(char));
//		}
//	}
//
//	//dlog_print(DLOG_INFO, LOG_TAG, "%s", ad->data.payload);
//	ad->data.mutex--;
//
//}
//
//
//
//
//
//static void
//log_message_pack(app_data_s *ad, char *str, int attempt){
//
//	// load settings
//	int err;
//	char key[RAPROTO_MAX_KEY_SIZE];
//
//	// implement mutex
//	while(ad->data.mutex != 0) {
//		usleep(RAPROTO_MUTEX_SLEEP);
//	}
//	ad->data.mutex++;
//	if (ad->data.mutex != 1){
//		dlog_print(DLOG_WARN, LOG_TAG, "mutex violation %d: %s", attempt, str); //TODO: add as warning
//		ad->data.mutex--;
//		attempt++;
//		if (attempt <= RAPROTO_MUTEX_ATTEMPTS) log_message_pack(ad, str, attempt);
//		return;
//	}
//
//	// Prepare for thingsboard (must start with '[')
//	if (strlen(ad->data.payload) == 0){
//		ad->data.payload[0] = '[';
//		ad->data.payload[1] = '\0';
//	} else if (strlen(ad->data.payload) >= ad->data.payloadmax - RAPROTO_MAX_MESSAGE_SIZE - 1){
//		int *temp;
//		size_t n_size;
//		if ((err = bundle_get_byte(ad->data.messages, RAPROTO_DATA_IDX_END, (void**)&temp, &n_size)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "retrieving idx end");
//		int idx_end = *temp;
//
//		increment_queue_idx(&idx_end, 1, RAPROTO_DATA_MAX_IDX);
//		if ((err = bundle_del(ad->data.messages, RAPROTO_DATA_IDX_END)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "deleting idx end");
//		if ((err = bundle_add_byte(ad->data.messages, RAPROTO_DATA_IDX_END, &idx_end, sizeof(int))) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "updating idx end");
//
//		sprintf(key, "%d", idx_end);
//		if ((err = bundle_add_str(ad->data.messages, key, ad->data.payload)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "adding payload to bundle");
//
//		dlog_print(DLOG_INFO, LOG_TAG, "message %s: len = %d ", key, strlen(ad->data.payload));
//
//		memset(ad->data.payload, 0, (ad->data.payloadmax) * sizeof(char));
//		ad->data.payload[0] = '[';
//		ad->data.payload[1] = '\0';
//	} else{
//		ad->data.payload[strlen(ad->data.payload)-1] = ',';
//	}
//
//	strcpy(&(ad->data.payload[strlen(ad->data.payload)]), str);
//
//	ad->data.payload[strlen(ad->data.payload)] = ']'; // must end with ']' for thingsboard
//	ad->data.payload[strlen(ad->data.payload)] = '\0'; // insert null character
//
//	//dlog_print(DLOG_INFO, LOG_TAG, "%s", ad->data.payload);
//	ad->data.mutex--;
//}
//
//
//static void
//log_stop(app_data_s *ad){
//
//	int err;
//	int val;
//
//	if ((err = bundle_del(ad->settings, RAPROTO_SETTING_LOGGING)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "del logging");
//	val = RAPROTO_SETTING_LOGGING_FALSE;
//	if ((err = bundle_add_byte(ad->settings, RAPROTO_SETTING_LOGGING, &val, sizeof(int))) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "add log byte");
//	config_publish(ad->settings);
//	ad->logging = false;
//
//	for (int j = 0; j < RAPROTO_SENSOR_MAX_NUM; j++){
//		if ((ad->sensors[j].monitor.on) && (j != RAPROTO_SENSOR_AUTOSAVE) && (j != RAPROTO_SENSOR_AUTOTRANSMIT) && (j != RAPROTO_SENSOR_BATTERY)) {
//			if ((err = sensor_destroy_listener(ad->sensors[j].listener)) != SENSOR_ERROR_NONE) error_msg(err, __func__, "listener destroy");
//		}
//		monitor_control(j, 1, RAPROTO_MONITOR_STATE_OFF, ad);
//	}
//
//	if ((err = device_power_release_lock(POWER_LOCK_CPU)) != DEVICE_ERROR_NONE) error_msg(err, __func__, "lock CPU release");
//
//	service_app_exit();
//}
//
//
//
//static Eina_Bool
//log_sensor_system(void *data){
//
//	app_data_s *ad = (app_data_s*)data;
//	char *device_id;
//	int err;
//	int battery_percent;
//	struct timeval t;
//	char msg[RAPROTO_MAX_MESSAGE_SIZE];
//
//	if (monitor_heart_beat(RAPROTO_SENSOR_BATTERY,ad)) {
//		if ((err = bundle_get_str(ad->settings, RAPROTO_SETTING_DEVICE_ID, &device_id)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "device id");
//
//		//TODO: gettimeofday is obsolete -- NEED TO FIX
//		gettimeofday(&t, NULL);
//
//		if ((err = device_battery_get_percent(&battery_percent)) != DEVICE_ERROR_NONE) error_msg(err, __func__, "get percentage");
//
//		sprintf(msg, "{\"ts\": \"%ld%ld\",\"values\"={\"%s_SYS\":{\"Bat\":%d}}}",
//			t.tv_sec,
//			t.tv_usec/1000,
//			device_id,
//			battery_percent);
//
//		log_message_pack(ad, msg, 0);
//
//		return ECORE_CALLBACK_RENEW;
//	} else {
//		return ECORE_CALLBACK_CANCEL;
//	}
//
//
//}
//
//
//
//
//static void
//log_sensor_accelerometer(sensor_h sensor, sensor_event_s *event, void *data)
//{
//	app_data_s *ad = (app_data_s*)data;
//	char *device_id;
//	struct timeval t;
//	char msg[RAPROTO_MAX_MESSAGE_SIZE];
//	int err;
//
//	if (monitor_heart_beat(RAPROTO_SENSOR_ACC, ad)) {
//		if ((err = bundle_get_str(ad->settings, RAPROTO_SETTING_DEVICE_ID, &device_id)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "device id");
//
//		//TODO: gettimeofday is obsolete -- NEED TO FIX
//		gettimeofday(&t, NULL);
//
//	    sensor_type_e type = SENSOR_ALL;
//	    if((sensor_get_type(sensor, &type) == SENSOR_ERROR_NONE) && type == SENSOR_ACCELEROMETER)
//	    {
//	    		sprintf(msg, "{\"ts\": \"%ld%ld\",\"values\"={\"%s_ACC\":{\"x\":%.2f,\"y\":%.2f,\"z\":%.2f}}}",
//	    			t.tv_sec,
//	    			t.tv_usec/1000,
//	    			device_id,
//				event->values[0],
//				event->values[1],
//				event->values[2]);
//	    }
//
//	    log_message_pack(ad, msg, 0);
//	}
//}
//
//
//
//
//static void
//log_sensor_setup(int idx, sensor_type_e type, const char *key, sensor_event_cb callback, int mode, app_data_s *ad){
//
//	int err;
//	bool sensor_supported = false;
//
//
//	if ((err = sensor_is_supported(type, &sensor_supported)) != SENSOR_ERROR_NONE) error_msg(err, __func__, "sensor supported");
//
//	if (sensor_supported){
//
//		int *log_period;
//		size_t acc_log_period_size;
//		if ((err = bundle_get_byte(ad->settings, key, (void**)&log_period, &acc_log_period_size)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "get key");
//
//		if (*log_period > 0) {
//
//			unsigned int pos_log = *log_period; // need unsigned for the log period
//			if (sensor_get_default_sensor(type, &(ad->sensors[idx].sensor)) == SENSOR_ERROR_NONE) {
//			   if (sensor_create_listener(ad->sensors[idx].sensor, &(ad->sensors[idx].listener)) == SENSOR_ERROR_NONE
//				   && sensor_listener_set_event_cb(ad->sensors[idx].listener, pos_log, callback, ad) == SENSOR_ERROR_NONE
//				   && sensor_listener_set_option(ad->sensors[idx].listener, SENSOR_OPTION_ALWAYS_ON) == SENSOR_ERROR_NONE
//				   && sensor_listener_set_attribute_int(ad->sensors[idx].listener, SENSOR_ATTRIBUTE_AXIS_ORIENTATION, SENSOR_AXIS_DEVICE_ORIENTED) == SENSOR_ERROR_NONE
//				   && sensor_listener_set_attribute_int(ad->sensors[idx].listener, SENSOR_ATTRIBUTE_PAUSE_POLICY, SENSOR_PAUSE_NONE) == SENSOR_ERROR_NONE)
//			   {
//				   if ((err = sensor_listener_start(ad->sensors[idx].listener)) != SENSOR_ERROR_NONE) error_msg(err, __func__, "listener start");
//			   } else{
//				   error_msg(err, __func__, "starting sensor");
//			   }
//			} else {
//				error_msg(err, __func__, "sensor default");
//			}
//
//			switch (mode){
//			case (0):
//				monitor_control(idx, 1, RAPROTO_MONITOR_STATE_ON, ad);
//				break;
//			case (1):
//				monitor_control(idx, *log_period, RAPROTO_MONITOR_STATE_ON, ad);
//				break;
//			default:
//				error_msg(APP_CONTROL_ERROR_IO_ERROR, __func__, "invalid mode");
//			}
//
//		}
//	}
//	return;
//}
//
//
//
//static void
//log_timer_setup(int idx, const char *setting, Ecore_Task_Cb func, app_data_s *ad) {
//	int *period;
//	size_t period_size;
//	bundle_get_byte(ad->settings, setting, (void**)&period, &period_size);
//	if (*period > 0) {
//		ecore_timer_add(*period, func, ad);
//		if ((idx != RAPROTO_SENSOR_AUTOSAVE) && (idx != RAPROTO_SENSOR_AUTOTRANSMIT)) {
//			monitor_control(idx, *period, RAPROTO_MONITOR_STATE_ON, ad);
//		}
//	}
//
//	if ((*period <= 0) && (!strcmp(RAPROTO_SETTING_BATTERY, setting))) error_msg(APP_CONTROL_ERROR_IO_ERROR, __func__, "battery must be positive");
//
//}
//
//
//
//void
//log_start(app_data_s *ad){
//
//	int err;
//	int val;
//
//	ecore_init();
//
//	// Setup data
//	data_setup(ad);
//
//	// Setup timers
//	log_timer_setup(RAPROTO_SENSOR_AUTOTRANSMIT, RAPROTO_SETTING_TX_RATE, transmit_autorequest, ad);
//	log_timer_setup(RAPROTO_SENSOR_AUTOSAVE, RAPROTO_SETTING_SAVE_RATE, data_autosave, ad);
//	log_timer_setup(RAPROTO_SENSOR_BATTERY, RAPROTO_SETTING_BATTERY, log_sensor_system, ad);
//
//	// Setup sensors
//	//TODO: add the other sensors
//	log_sensor_setup(RAPROTO_SENSOR_ACC, SENSOR_ACCELEROMETER, RAPROTO_SETTING_SENSOR_ACC, log_sensor_accelerometer, 0, ad);
//
//
//	//if ((err = device_display_set_brightness(0,1)) != DEVICE_ERROR_NONE) error_msg(err, __func__, "brightness");
//	//if ((err = device_display_change_state(DISPLAY_STATE_SCREEN_OFF)) != DEVICE_ERROR_NONE) error_msg(err, __func__, "screen state");
//	if ((err = device_power_request_lock(POWER_LOCK_CPU, 0)) != DEVICE_ERROR_NONE) error_msg(err, __func__, "lock CPU");
//
//	dlog_print(DLOG_INFO, LOG_TAG, "monitoring started");
//
//	if ((err = bundle_del(ad->settings, RAPROTO_SETTING_LOGGING)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "del logging");
//	val = RAPROTO_SETTING_LOGGING_TRUE;
//	if ((err = bundle_add_byte(ad->settings, RAPROTO_SETTING_LOGGING, &val, sizeof(int))) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "add log byte");
//	ad->logging = true;
//
//	task_process_cb(RAPROTO_TASK_LOG_DONE);
//
//	ecore_main_loop_begin();
//}
//


//
//
//static void
//task_msg(const char *msg, app_data_s *ad){
//	int err;
//
//	switch (ad->scheduler.active){
//		case RAPROTO_TASK_UPDATE:
//			if ((err = bundle_del(ad->settings, RAPROTO_SETTING_CONFIG_STATUS)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "bundle del config");
//			if ((err = bundle_add_str(ad->settings, RAPROTO_SETTING_CONFIG_STATUS, msg)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "config error");
//			break;
//		case RAPROTO_TASK_LOG_ON:
//			break;
//		case RAPROTO_TASK_TRANSMIT:
//			if ((err = bundle_del(ad->settings, RAPROTO_SETTING_LAST_SYNC)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "bundle del transmit");
//			if ((err = bundle_add_str(ad->settings, RAPROTO_SETTING_LAST_SYNC, msg)) != BUNDLE_ERROR_NONE) error_msg(err, __func__, "transmit error");
//			break;
//		case RAPROTO_TASK_NONE:
//			return;
//		default:
//			error_msg(APP_CONTROL_ERROR_KEY_NOT_FOUND, __func__, "unknown task");
//			break;
//	}
//
//	return;
//}
//
//
//void
//task_error(const char *msg, app_data_s *ad){
//
//	task_msg(msg, ad);
//
//	error_msg(APP_CONTROL_ERROR_IO_ERROR, __func__, msg);
//
//	return;
//}
//
//
//void
//task_warn(const char *msg, app_data_s *ad){
//
//	warn_msg(APP_CONTROL_ERROR_IO_ERROR, __func__, msg);
//
//	task_msg(msg,ad);
//
//	config_publish(ad->settings);
//	wifi_stop(ad);
//
//	return;
//}
//
//
//
//static void
//task_stop(app_data_s *ad){
//	config_publish(ad->settings); // publish configuration to main app
//
//	dlog_print(DLOG_INFO, LOG_TAG, "task complete: %d", ad->scheduler.active);
//	ad->scheduler.tasks[ad->scheduler.active] = RAPROTO_TASK_SCHEDULE_OFF;
//	ad->scheduler.active = RAPROTO_TASK_NONE;
//	task_start(ad);
//	return;
//}
//
//
//
//static void
//task_process_cb(const char* event){
//	int err;
//	bundle *b = bundle_create();
//	if ((err = event_publish_app_event(event, b)) != EVENT_ERROR_NONE) error_msg(err,__func__, NULL);
//	bundle_free(b);
//}
//
//
////TODO: Make sure all tasks have a timeout and graceful exit
//void
//task_process(const char *event_name, bundle *junk, void *data){
//	app_data_s *ad = (app_data_s*)data;
//
//	dlog_print(DLOG_INFO, LOG_TAG, "task_process: %d : %s", ad->scheduler.active, event_name);
//
//	switch(ad->scheduler.active){
//		case (RAPROTO_TASK_UPDATE):
//			if (!strcmp(event_name, RAPROTO_TASK_START))	wifi_start(ad);
//			else if (!strcmp(event_name, RAPROTO_TASK_WIFI_ON_DONE)) mqtt_start(ad);
//			else if (!strcmp(event_name, RAPROTO_TASK_MQTT_ON_DONE)) config_start(ad);
//			else if (!strcmp(event_name, RAPROTO_TASK_CONFIG_DONE)) mqtt_stop(ad);
//			else if (!strcmp(event_name, RAPROTO_TASK_MQTT_OFF_DONE)) mqtt_destroy(ad);
//			else if (!strcmp(event_name, RAPROTO_TASK_MQTT_DESTROY_DONE)) wifi_stop(ad);
//			else if (!strcmp(event_name, RAPROTO_TASK_WIFI_OFF_DONE)) task_stop(ad);
//			return;
//		case (RAPROTO_TASK_LOG_ON):
//			if (!strcmp(event_name, RAPROTO_TASK_START)) log_start(ad);
//			else if (!strcmp(event_name, RAPROTO_TASK_LOG_DONE)) wifi_stop(ad);
//			else if (!strcmp(event_name, RAPROTO_TASK_WIFI_OFF_DONE)) task_stop(ad);
//			return;
//		case (RAPROTO_TASK_TRANSMIT):
//			if (!strcmp(event_name, RAPROTO_TASK_START)) wifi_start(ad);
//			else if (!strcmp(event_name, RAPROTO_TASK_WIFI_ON_DONE)) mqtt_start(ad);
//			else if (!strcmp(event_name, RAPROTO_TASK_MQTT_ON_DONE)) transmit_data_setup(ad);
//			else if (!strcmp(event_name, RAPROTO_TASK_TRANSMIT_SETUP_DONE)) transmit_start(ad);
//			else if (!strcmp(event_name, RAPROTO_TASK_TRANSMIT_DONE)) mqtt_stop(ad);
//			else if (!strcmp(event_name, RAPROTO_TASK_MQTT_OFF_DONE)) mqtt_destroy(ad);
//			else if (!strcmp(event_name, RAPROTO_TASK_MQTT_DESTROY_DONE)) wifi_stop(ad);
//			else if (!strcmp(event_name, RAPROTO_TASK_WIFI_OFF_DONE)) task_stop(ad);
//			return;
//		case (RAPROTO_TASK_STOP_SOFT):
//			service_app_exit();
//			return;
//		default:
//			dlog_print(DLOG_ERROR, LOG_TAG, "unknown task number: %d", ad->scheduler.active);
//			task_stop(ad);
//	}
//
//	return;
//}
//
//
//
//
//void
//task_start(app_data_s *ad){
//	int j;
//
//	if (ad->scheduler.active == RAPROTO_TASK_NONE) {
//
//		for (j = 0; j < RAPROTO_TASK_NUMBER; j++){
//			if (ad->scheduler.tasks[j] == RAPROTO_TASK_SCHEDULE_ON){
//				dlog_print(DLOG_INFO, LOG_TAG, "Starting task: %d", j);
//				ad->scheduler.active = j;
//				task_process_cb(RAPROTO_TASK_START);
//				return;
//			}
//		}
//
//	} else {
//		dlog_print(DLOG_INFO, LOG_TAG, "Task already in process: %d", ad->scheduler.active);
//	}
//	return;
//}
//




void
service_app_terminate(void *data)
{

	dlog_print(DLOG_INFO, LOG_TAG, "app terminate called");

    app_data_s* ad = (app_data_s*)data;

    // Stop logging
    log_stop(ad);

    if (!(ad->reset)) {
    		// Store all non-transmitted data
    		log_message_pack_all(ad,0);
    		data_save(ad->data.messages, RAPROTO_DATA_FILENAME);

    		// Save all settings
    		data_save(ad->settings, RAPROTO_SETTING_FILENAME);
    } else {

		char path[MAX_FILE_PATH];

		bundle_free(ad->settings);
		ad->settings = NULL;
		sprintf(path,"%s%s\n",app_get_data_path(), RAPROTO_SETTING_FILENAME);
		remove(path);

		log_message_pack_all(ad,0);
		bundle_free(ad->data.messages);
		ad->data.messages = NULL;
		sprintf(path,"%s%s\n",app_get_data_path(), RAPROTO_DATA_FILENAME);
		remove(path);
    }

    // free MQTT
	if (ad->mqtt.client != NULL) {
		MQTTAsync_destroy(ad->mqtt.client);
		free(ad->mqtt.client);
		ad->mqtt.client = NULL;
	}

	// free WIFI
    wifi_autoconnect_destroy(&(ad->wifi_ac));
    wifi_deinitialize(ad);

    	// release power lock
    device_power_release_lock(POWER_LOCK_CPU);

    dlog_print(DLOG_INFO, LOG_TAG, "app terminate");
}


void
service_stop_cb(const char *event_name, bundle *settings, void *data){
	app_data_s *ad = (app_data_s*)data;
	service_app_terminate(ad);
}

bool
service_app_create(void *data)
{
	dlog_print(DLOG_INFO, LOG_TAG, "app create");

	app_data_s* ad = (app_data_s*)data;

    // factory reset false
	ad->reset = false;
	ad->logging = false;

	// settings initialization
	ad->settings = data_load_settings();

	// Initialize monitors (must be done after loading settings)
	monitor_init(ad);

	// Scheduler initializations
    ad->scheduler.active = RAPROTO_TASK_NONE;

	// Wifi initializations
	ad->wifi = NULL;
	wifi_initialize(ad);

	// MQTT initializations
    ad->mqtt.client = NULL;

    // message initialization
    ad->data.initialized = false;


	return true;
}


static void
service_app_factory_reset(app_data_s *ad){
	ad->reset = true;
	service_app_exit();
}



static void
service_app_control(app_control_h request, void *data)
{
	/* APP_EVENT_CONTROL */
	app_data_s *ad = (app_data_s*)data;

	int err;
	app_control_h reply;
	char *operation = NULL;
	char *task = NULL;

	if ((err = app_control_get_operation(request, &operation)) != APP_CONTROL_ERROR_NONE) error_msg(err, __func__, "get operation");

	if(!strcmp(operation, APP_CONTROL_OPERATION_DEFAULT)) {

		if ((err = app_control_get_extra_data(request, RAPROTO_TASK_REQUEST, &task)) != APP_CONTROL_ERROR_NONE) error_msg(err, __func__, "get extra data");
		if (strcmp(task,RAPROTO_TASK_REQUEST_LOG_CHECK)) dlog_print(DLOG_INFO, LOG_TAG, "Scheduling task: %s", task);



		if (!strcmp(task, RAPROTO_TASK_REQUEST_START)) {
			config_publish(ad->settings);
		} else if (!strcmp(task, RAPROTO_TASK_REQUEST_STOP)) {
			service_app_exit();
		} else if (!strcmp(task, RAPROTO_TASK_REQUEST_SETTINGS_TRANSMIT)) {
			data_load_user(RAPROTO_SETTING_DEVICE_ID, request, ad);
			data_load_user(RAPROTO_SETTING_MQTT_BROKER, request, ad);
			data_load_user(RAPROTO_SETTING_MQTT_USERNAME, request, ad);
			data_load_user(RAPROTO_SETTING_MQTT_PASSWORD, request, ad);
			data_load_user(RAPROTO_SETTING_MQTT_CONFIG_PUB_TOPIC, request, ad);
			data_load_user(RAPROTO_SETTING_MQTT_CONFIG_SUB_TOPIC, request, ad);
			config_publish(ad->settings);
		} else if (!strcmp(task, RAPROTO_TASK_REQUEST_ERROR)) {
			if (!(ad->reset)) error_msg(APP_CONTROL_ERROR_NONE, __func__, "error request from main app");
		} else if (!strcmp(task, RAPROTO_TASK_REQUEST_FACTORY_RESET)) {
			service_app_factory_reset(ad);
		} else if (!strcmp(task, RAPROTO_TASK_REQUEST_LOG_ON)) {
			ad->scheduler.tasks[RAPROTO_TASK_LOG_ON] = RAPROTO_TASK_SCHEDULE_ON;
		} else if (!strcmp(task, RAPROTO_TASK_REQUEST_LOG_OFF)) {
			return log_stop(ad);
		} else if (!strcmp(task, RAPROTO_TASK_REQUEST_LOG_CHECK)){
			if (!monitor_cb(ad)) return;
		} else if (!strcmp(task, RAPROTO_TASK_REQUEST_SETTINGS_UPDATE)) {
			if (!(ad->logging)){
				config_status(RAPROTO_UPDATE_INPROGRESS_MESSAGE, ad->settings);
				ad->scheduler.tasks[RAPROTO_TASK_UPDATE] = RAPROTO_TASK_SCHEDULE_ON;
			}
		} else if (!strcmp(task, RAPROTO_TASK_REQUEST_TRANSMIT)) {
			ad->scheduler.tasks[RAPROTO_TASK_TRANSMIT] = RAPROTO_TASK_SCHEDULE_ON;
		} else if (!strcmp(task, RAPROTO_TASK_REQUEST_STOP_SOFT)) {
			ad->scheduler.tasks[RAPROTO_TASK_STOP_SOFT] = RAPROTO_TASK_SCHEDULE_ON;
		} else {
			error_msg(BUNDLE_ERROR_INVALID_PARAMETER, __func__, "unknown task");
		}

		if ((err = app_control_create(&reply)) != APP_CONTROL_ERROR_NONE) return error_msg(err,__func__,"create");
		if ((err = app_control_set_app_id(reply, RAPROTO_APP_ID)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__, "app_id");
		if ((err = app_control_reply_to_launch_request(reply, request, APP_CONTROL_RESULT_SUCCEEDED)) != APP_CONTROL_ERROR_NONE) return error_msg(err, __func__,"reply");
		if ((err = app_control_destroy(reply)) != APP_CONTROL_ERROR_NONE) return error_msg(err,__func__,"destroy");

		task_start(ad);
	}

	//dlog_print(DLOG_INFO, LOG_TAG, "App control complete");

	return;
}


static void
service_app_lang_changed(app_event_info_h event_info, void *data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
}


static void
service_app_region_changed(app_event_info_h event_info, void *data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
service_app_low_battery(app_event_info_h event_info, void *data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void
service_app_low_memory(app_event_info_h event_info, void *data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

int main(int argc, char* argv[])
{
	int err;

	// initialize application data
    app_data_s ad = {0,};

	service_app_lifecycle_callback_s event_callback;
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = service_app_create;
	event_callback.terminate = service_app_terminate;
	event_callback.app_control = service_app_control;

	service_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, service_app_low_battery, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, service_app_low_memory, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, service_app_lang_changed, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, service_app_region_changed, &ad);


	event_handler_h task_start_h;
	if ((err = event_add_event_handler(RAPROTO_TASK_START, task_process, &ad, &task_start_h)) != EVENT_ERROR_NONE) error_msg(err, __func__,"start");

	event_handler_h task_monitor_on_h, task_monitor_off_h;
	if ((err = event_add_event_handler(RAPROTO_TASK_MONITOR_ON_DONE, task_process, &ad, &task_monitor_on_h)) != EVENT_ERROR_NONE) error_msg(err, __func__,"task monitor on");
	if ((err = event_add_event_handler(RAPROTO_TASK_MONITOR_OFF_DONE, task_process, &ad, &task_monitor_off_h)) != EVENT_ERROR_NONE) error_msg(err, __func__,"task monitor off");

	event_handler_h task_wifi_on_h, task_wifi_off_h;
	if ((err = event_add_event_handler(RAPROTO_TASK_WIFI_ON_DONE, task_process, &ad, &task_wifi_on_h)) != EVENT_ERROR_NONE) error_msg(err, __func__, "wifi on");
	if ((err = event_add_event_handler(RAPROTO_TASK_WIFI_OFF_DONE, task_process, &ad, &task_wifi_off_h)) != EVENT_ERROR_NONE) error_msg(err, __func__, "wifi off");

	event_handler_h task_mqtt_on_h, task_mqtt_off_h, task_mqtt_destroy_h;
	if ((err = event_add_event_handler(RAPROTO_TASK_MQTT_ON_DONE, task_process, &ad, &task_mqtt_on_h)) != EVENT_ERROR_NONE) error_msg(err, __func__, "mqtt on");
	if ((err = event_add_event_handler(RAPROTO_TASK_MQTT_OFF_DONE, task_process, &ad, &task_mqtt_off_h)) != EVENT_ERROR_NONE) error_msg(err, __func__, "mqtt off");
	if ((err = event_add_event_handler(RAPROTO_TASK_MQTT_DESTROY_DONE, task_process, &ad, &task_mqtt_destroy_h)) != EVENT_ERROR_NONE) error_msg(err, __func__, "mqtt destroy");

	event_handler_h task_transmit_h, task_transmit_setup_h, task_config_h, task_log_h;
	if ((err = event_add_event_handler(RAPROTO_TASK_TRANSMIT_DONE, task_process, &ad, &task_transmit_h)) != EVENT_ERROR_NONE) error_msg(err, __func__, "transmit");
	if ((err = event_add_event_handler(RAPROTO_TASK_TRANSMIT_SETUP_DONE, task_process, &ad, &task_transmit_setup_h)) != EVENT_ERROR_NONE) error_msg(err, __func__, "transmit");
	if ((err = event_add_event_handler(RAPROTO_TASK_CONFIG_DONE, task_process, &ad, &task_config_h)) != EVENT_ERROR_NONE) error_msg(err, __func__, "config");
	if ((err = event_add_event_handler(RAPROTO_TASK_LOG_DONE, task_process, &ad, &task_log_h)) != EVENT_ERROR_NONE) error_msg(err, __func__, "monitor");

	event_handler_h error_event_h;
	if ((err = event_add_event_handler(RAPROTO_ERROR_EVENT, error_msg_event, &ad, &error_event_h)) != EVENT_ERROR_NONE) error_msg(err, __func__,"start");

	return service_app_main(argc, argv, &event_callback, &ad);

}
