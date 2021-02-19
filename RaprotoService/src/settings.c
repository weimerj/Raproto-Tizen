/*
 * settings.c
 *
 *  Created on: Feb 14, 2021
 *      Author: weimer
 */


#include "settings.h"




bundle*
settings_init(){
	bundle *settings;
	settings = bundle_create();
	char *val_str;
	char val_str_truncated[RAPROTO_TIZEN_ID_KEY_LENGTH+1];
	int val;

	// default data that is manually updated
	system_info_get_platform_string(RAPROTO_TIZEN_ID_KEY, &val_str);
	strncpy(val_str_truncated,val_str,RAPROTO_TIZEN_ID_KEY_LENGTH);
	bundle_add_str(settings, RAPROTO_SETTING_DEVICE_ID, val_str_truncated);
	bundle_add_str(settings, RAPROTO_SETTING_MQTT_USERNAME, val_str_truncated);
	free(val_str);

	bundle_add_str(settings, RAPROTO_SETTING_MQTT_BROKER, RAPROTO_SETTING_MQTT_BROKER_DEFAULT);
	bundle_add_str(settings, RAPROTO_SETTING_MQTT_CONFIG_SUB_TOPIC, RAPROTO_SETTING_MQTT_CONFIG_SUB_TOPIC_DEFAULT);
	bundle_add_str(settings, RAPROTO_SETTING_MQTT_CONFIG_PUB_TOPIC, RAPROTO_SETTING_MQTT_CONFIG_PUB_TOPIC_DEFAULT);
	bundle_add_str(settings, RAPROTO_SETTING_MQTT_PASSWORD, RAPROTO_SETTING_MQTT_PASSWORD_DEFAULT);

	// default data that is updated through auto-update
	bundle_add_str(settings, RAPROTO_SETTING_NAME, RAPROTO_SETTING_NAME_DEFAULT);
	bundle_add_str(settings, RAPROTO_SETTING_MQTT_PUB_TOPIC, RAPROTO_SETTING_MQTT_PUB_TOPIC_DEFAULT);
	val = RAPROTO_SETTING_POWER_DEFAULT;
	bundle_add_byte(settings, RAPROTO_SETTING_POWER, &val, sizeof(int));
	val = RAPROTO_SETTING_WIFI_DEFAULT;
	bundle_add_byte(settings, RAPROTO_SETTING_WIFI, &val, sizeof(int));
	val = RAPROTO_SETTING_WIFI_TIMEOUT_DEFAULT;
	bundle_add_byte(settings, RAPROTO_SETTING_WIFI_TIMEOUT, &val, sizeof(int));
	val = RAPROTO_SETTING_MQTT_TIMEOUT_DEFAULT;
	bundle_add_byte(settings, RAPROTO_SETTING_MQTT_TIMEOUT, &val, sizeof(int));
	val = RAPROTO_SETTING_MQTT_MAX_PAYLOAD_SIZE_DEFAULT;
	bundle_add_byte(settings, RAPROTO_SETTING_MQTT_MAX_PAYLOAD_SIZE, &val, sizeof(int));
	val = RAPROTO_SETTING_MQTT_QOS_DEFAULT;
	bundle_add_byte(settings, RAPROTO_SETTING_MQTT_QOS, &val, sizeof(int));
	val = RAPROTO_SETTING_MQTT_KEEP_ALIVE_DEFAULT;
	bundle_add_byte(settings, RAPROTO_SETTING_MQTT_KEEP_ALIVE, &val, sizeof(int));
	val = RAPROTO_SETTING_TX_RATE_DEFAULT;
	bundle_add_byte(settings, RAPROTO_SETTING_TX_RATE, &val, sizeof(int));
	val = RAPROTO_SETTING_BATTERY_DEFAULT;
	bundle_add_byte(settings, RAPROTO_SETTING_BATTERY, &val, sizeof(int));
	val = RAPROTO_SETTING_SAVE_RATE_DEFAULT;
	bundle_add_byte(settings, RAPROTO_SETTING_SAVE_RATE, &val, sizeof(int));
	val = RAPROTO_SETTING_HEART_BEAT_DEFAULT;
	bundle_add_byte(settings, RAPROTO_SETTING_HEART_BEAT, &val, sizeof(int));
	val = RAPROTO_SETTING_ERROR_FALSE;
	bundle_add_byte(settings, RAPROTO_SETTING_ERROR, &val, sizeof(int));
	val = RAPROTO_SETTING_LOGGING_FALSE;
	bundle_add_byte(settings, RAPROTO_SETTING_LOGGING, &val, sizeof(int));

	val = RAPROTO_SETTING_SENSOR_GENERIC_DEFAULT;
	bundle_add_byte(settings, RAPROTO_SETTING_SENSOR_ACC, &val, sizeof(int));
	bundle_add_byte(settings, RAPROTO_SETTING_SENSOR_HRM, &val, sizeof(int));
	bundle_add_byte(settings, RAPROTO_SETTING_SENSOR_PPG, &val, sizeof(int));
	bundle_add_byte(settings, RAPROTO_SETTING_SENSOR_TEMP, &val, sizeof(int));
	bundle_add_byte(settings, RAPROTO_SETTING_SENSOR_SLEEP, &val, sizeof(int));
	bundle_add_byte(settings, RAPROTO_SETTING_SENSOR_STEPS, &val, sizeof(int));
	bundle_add_byte(settings, RAPROTO_SETTING_SENSOR_STRESS, &val, sizeof(int));
	bundle_add_byte(settings, RAPROTO_SETTING_SENSOR_GYRO, &val, sizeof(int));
	bundle_add_byte(settings, RAPROTO_SETTING_SENSOR_MAG, &val, sizeof(int));

	// data that is updated by the software (only)
	bundle_add_str(settings, RAPROTO_SETTING_LAST_UPDATE, RAPROTO_SETTING_LAST_UPDATE_DEFAULT);
	bundle_add_str(settings, RAPROTO_SETTING_CONFIG_STATUS, RAPROTO_SETTING_CONFIG_STATUS_DEFAULT);
	bundle_add_str(settings, RAPROTO_SETTING_LAST_SYNC, RAPROTO_SETTING_LAST_SYNC_DEFAULT);
	bundle_add_str(settings, RAPROTO_SETTING_MQTT_CLIENT_ID, RAPROTO_SETTING_MQTT_CLIENT_ID_DEFAULT);

	char str[1000];
	sprintf(str,"{\"sharedKeys\":\"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\"}",
		RAPROTO_SETTING_NAME,
		RAPROTO_SETTING_POWER,
		RAPROTO_SETTING_WIFI,
		RAPROTO_SETTING_WIFI_TIMEOUT,
		RAPROTO_SETTING_MQTT_PUB_TOPIC,
		RAPROTO_SETTING_MQTT_QOS,
		RAPROTO_SETTING_MQTT_KEEP_ALIVE,
		RAPROTO_SETTING_MQTT_TIMEOUT,
		RAPROTO_SETTING_MQTT_MAX_PAYLOAD_SIZE,
		RAPROTO_SETTING_SENSOR_ACC,
		RAPROTO_SETTING_SENSOR_HRM,
		RAPROTO_SETTING_SENSOR_PPG,
		RAPROTO_SETTING_SENSOR_TEMP,
		RAPROTO_SETTING_SENSOR_SLEEP,
		RAPROTO_SETTING_SENSOR_STEPS,
		RAPROTO_SETTING_SENSOR_STRESS,
		RAPROTO_SETTING_SENSOR_GYRO,
		RAPROTO_SETTING_SENSOR_MAG,
		RAPROTO_SETTING_TX_RATE,
		RAPROTO_SETTING_BATTERY,
		RAPROTO_SETTING_SAVE_RATE,
		RAPROTO_SETTING_HEART_BEAT);
	bundle_add_str(settings, RAPROTO_SETTING_MQTT_CONFIG_PUB_MSG, str);

	return settings;
}
