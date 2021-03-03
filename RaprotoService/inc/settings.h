/*
 * settings.h
 *
 *  Created on: Feb 13, 2021
 *      Author: weimer
 */


//#define RELEASE_NEURALERT

#ifndef SETTINGS_H_
#define SETTINGS_H_

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "RaprotoService"

#define RAPROTO_TASK_SCHEDULE_TASK				"event.org.precise.raproto.service.schedule_task"
#define RAPROTO_TASK_START						"event.org.precise.raproto.service.task_start"

#define RAPROTO_TASK_SAVE_DONE					"event.org.precise.raproto.service.task_save"

#define RAPROTO_TASK_MONITOR_ON_DONE				"event.org.precise.raproto.service.task_monitor_on"
#define RAPROTO_TASK_MONITOR_OFF_DONE			"event.org.precise.raproto.service.task_monitor_off"

#define RAPROTO_TASK_WIFI_ON_DONE				"event.org.precise.raproto.service.task_wifi_on"
#define RAPROTO_TASK_WIFI_OFF_DONE				"event.org.precise.raproto.service.task_wifi_off"

#define RAPROTO_TASK_MQTT_ON_DONE				"event.org.precise.raproto.service.task_mqtt_on"
#define RAPROTO_TASK_MQTT_OFF_DONE				"event.org.precise.raproto.service.task_mqtt_off"
#define RAPROTO_TASK_MQTT_DESTROY_DONE			"event.org.precise.raproto.service.task_mqtt_destroy"

#define RAPROTO_TASK_CONFIG_DONE					"event.org.precise.raproto.service.task_config"
#define RAPROTO_TASK_TRANSMIT_DONE				"event.org.precise.raproto.service.task_transmit"
#define RAPROTO_TASK_TRANSMIT_SETUP_DONE			"event.org.precise.raproto.service.task_transmit_setup"
#define RAPROTO_TASK_LOG_DONE					"event.org.precise.raproto.service.task_log"

#define RAPROTO_TASK_FEEDBACK_DONE				"event.org.precise.raproto.service.task_feedback"


#define RAPROTO_TOPIC_SETTINGS					"event.org.precise.raproto.service.publish_settings"
//#define RAPROTO_TOPIC_HEART_BEAT					"event.org.precise.raproto.service.publish_heart_beat"

#define RAPROTO_ERROR_EVENT						"event.org.precise.raproto.service.error_event"
#define RAPROTO_TASK_ERROR_EVENT					"event.org.precise.raproto.service.task_error_event"
#define RAPROTO_TASK_WARN_EVENT					"event.org.precise.raproto.service.task_warn_event"



#define RAPROTO_TIZEN_ID_KEY						"http://tizen.org/system/tizenid"
#define RAPROTO_TIZEN_ID_KEY_LENGTH				10

//#define EVENT_RAPROTO_TX_BEGIN 			"event.org.precise.raproto.service.tx_begin"
//#define EVENT_RAPROTO_TX_DONE 			"event.org.precise.raproto.service.tx_done"
//#define EVENT_RAPROTO_WIFI_ON 			"event.org.precise.raproto.service.wifi_on"
//#define EVENT_RAPROTO_WIFI_OFF 			"event.org.precise.raproto.service.wifi_off"
//#define EVENT_RAPROTO_STATUS 			"event.org.precise.raproto.service.status"
//#define EVENT_RAPROTO_CONFIG_DONE		"event.org.precise.raproto.service.config_done"
//#define EVENT_RAPROTO_CONFIG_BEGIN		"event.org.precise.raproto.ui.config_begin"
//#define EVENT_RAPROTO_TX_NOW				"event.org.precise.raproto.ui.tx_now"
//#define EVENT_RAPROTO_START_MONITORING	"event.org.precise.raproto.ui.start"
//#define EVENT_RAPROTO_STOP				"event.org.precise.raproto.ui.stop"




#define STRNCMP_LIMIT 256 // the limit of characters to be compared using strncmp function

#define MAX_KEY_LENGTH  10


//TODO: turn all these into enums (Raproto error below)
#define RAPROTO_MAX_NUM_OF_SETTINGS 				50

#define RAPROTO_SETTING_NAME 					"NAME"
#define RAPROTO_SETTING_TX_RATE					"TX_RATE"
#define RAPROTO_SETTING_SAVE_RATE				"SAVE_RATE"
#define RAPROTO_SETTING_POWER					"POWER"
#define RAPROTO_SETTING_WIFI						"WIFI"
#define RAPROTO_SETTING_WIFI_TIMEOUT				"WIFI_TIMEOUT"
#define RAPROTO_SETTING_DEVICE_ID 				"DEVICE_ID"
#define RAPROTO_SETTING_MQTT_CLIENT_ID			"CLIENT"
#define RAPROTO_SETTING_MQTT_BROKER				"BROKER"
#define RAPROTO_SETTING_MQTT_QOS					"QOS"
#define RAPROTO_SETTING_MQTT_KEEP_ALIVE			"KEEP_ALIVE"
#define RAPROTO_SETTING_MQTT_CONFIG_SUB_TOPIC	"CONFIG_SUB"
#define RAPROTO_SETTING_MQTT_CONFIG_PUB_TOPIC	"CONFIG_PUB"
#define RAPROTO_SETTING_MQTT_CONFIG_PUB_MSG		"CONFIG_PUB_MSG"
#define RAPROTO_SETTING_MQTT_PUB_TOPIC			"PUB"
#define RAPROTO_SETTING_MQTT_TIMEOUT				"MQTT_TIMEOUT"
#define RAPROTO_SETTING_MQTT_MAX_PAYLOAD_SIZE	"MAX_PAYLOAD_SIZE"
#define RAPROTO_SETTING_MQTT_USERNAME			"USERNAME"
#define RAPROTO_SETTING_MQTT_PASSWORD			"PASSWORD"
#define RAPROTO_SETTING_LAST_UPDATE				"UPDATE"
#define RAPROTO_SETTING_CONFIG_STATUS			"CONFIG_STATUS"
#define RAPROTO_SETTING_LAST_SYNC				"SYNC"
#define RAPROTO_SETTING_BATTERY					"BATTERY"
#define RAPROTO_SETTING_HEART_BEAT				"HEART_BEAT"
#define RAPROTO_SETTING_LOGGING					"SERVICE_LOGGING"
#define RAPROTO_SETTING_ERROR					"SERVICE_ERROR"

#define RAPROTO_SETTING_FILENAME					"raproto_config.txt"


// DEFAULT VALUES

#define RAPROTO_SETTING_TX_RATE_DEFAULT						60
#define RAPROTO_SETTING_SAVE_RATE_DEFAULT					60
#define RAPROTO_SETTING_POWER_DEFAULT						0
#define RAPROTO_SETTING_WIFI_DEFAULT							1
#define RAPROTO_SETTING_WIFI_TIMEOUT_DEFAULT					10
#define RAPROTO_SETTING_MQTT_CLIENT_ID_DEFAULT 				"Raproto"
#define RAPROTO_SETTING_MQTT_QOS_DEFAULT						1
#define RAPROTO_SETTING_MQTT_KEEP_ALIVE_DEFAULT				30
#define RAPROTO_SETTING_MQTT_CONFIG_SUB_TOPIC_DEFAULT		"v1/devices/me/attributes/response/+"
#define RAPROTO_SETTING_MQTT_CONFIG_PUB_TOPIC_DEFAULT		"v1/devices/me/attributes/request/1"
#define RAPROTO_SETTING_MQTT_PUB_TOPIC_DEFAULT				"v1/devices/me/telemetry"
#define RAPROTO_SETTING_MQTT_TIMEOUT_DEFAULT					10000L
#define RAPROTO_SETTING_MQTT_MAX_PAYLOAD_SIZE_DEFAULT 		10000
#define RAPROTO_SETTING_MQTT_PASSWORD_DEFAULT				"\0"		// No password for ThingsBoard
#define RAPROTO_SETTING_LAST_UPDATE_DEFAULT 					"never"
#define RAPROTO_SETTING_LAST_SYNC_DEFAULT					"never"
#define RAPROTO_SETTING_CONFIG_STATUS_DEFAULT				"status: unknown"
#define RAPROTO_SETTING_BATTERY_DEFAULT						10
#define RAPROTO_SETTING_SENSOR_GENERIC_DEFAULT				-1 // negative means "off"
#define RAPROTO_SETTING_HEART_BEAT_DEFAULT					1
#define RAPROTO_SETTING_LOGGING								"SERVICE_LOGGING"
#define RAPROTO_SETTING_ERROR								"SERVICE_ERROR"
#define RAPROTO_SETTING_MQTT_BROKER_DEFAULT					"ssl://tb.precise.seas.upenn.edu:8883"
#define RAPROTO_SETTING_NAME_DEFAULT							"\0"


#define RAPROTO_SETTING_SENSOR_ACC			"ACC"
#define RAPROTO_SETTING_SENSOR_HRM			"HRM"
#define RAPROTO_SETTING_SENSOR_HRM_GREEN		"HRM_GREEN"
#define RAPROTO_SETTING_SENSOR_TEMP			"TEMP"
#define RAPROTO_SETTING_SENSOR_SLEEP			"SLEEP"
#define RAPROTO_SETTING_SENSOR_STEPS			"STEPS"
#define RAPROTO_SETTING_SENSOR_STRESS		"STRESS"
#define RAPROTO_SETTING_SENSOR_GYRO 			"GYRO"
#define RAPROTO_SETTING_SENSOR_GYRO_ROTATION	"ROTATION"
#define RAPROTO_SETTING_SENSOR_MAG			"MAG"
#define RAPROTO_SETTING_SENSOR_GRAVITY		"GRAVITY"



#define RAPROTO_TASK_REQUEST								"TASK_REQUEST"
#define RAPROTO_TASK_REQUEST_START						"START_SERVICE"
#define RAPROTO_TASK_REQUEST_STOP						"STOP_SERVICE"
#define RAPROTO_TASK_REQUEST_STOP_SOFT					"SOFT_STOP_SERVICE"
#define RAPROTO_TASK_REQUEST_SETTINGS_UPDATE				"SETTINGS_UPDATE"
#define RAPROTO_TASK_REQUEST_TRANSMIT					"TRANSMIT_DATA"
#define RAPROTO_TASK_REQUEST_LOG_ON						"LOG_ON"
#define RAPROTO_TASK_REQUEST_LOG_OFF						"LOG_OFF"
#define RAPROTO_TASK_REQUEST_LOG_CHECK					"LOG_CHECK"
#define RAPROTO_TASK_REQUEST_SETTINGS_TRANSMIT		    	"SETTINGS_TRANSMIT"
#define RAPROTO_TASK_REQUEST_ERROR						"ERROR"
#define RAPROTO_TASK_REQUEST_FACTORY_RESET				"FACTORY_RESET"

//TODO: Turn these into enums
#define RAPROTO_TASK_NUMBER					4
#define RAPROTO_TASK_NONE					-1
#define RAPROTO_TASK_UPDATE					0
#define RAPROTO_TASK_TRANSMIT				1
#define RAPROTO_TASK_LOG_ON					2
#define RAPROTO_TASK_STOP_SOFT				3

#define RAPROTO_TASK_SCHEDULE_OFF			0
#define RAPROTO_TASK_SCHEDULE_ON				1


#define RAPROTO_WIFI_CONTROL_NONE			0
#define RAPROTO_WIFI_CONTROL_ON_ONLY			1
#define RAPROTO_WIFI_CONTROL_FULL			2


#define RAPROTO_SENSOR_MAX_NUM	 			25
#define RAPROTO_SENSOR_NONE					-1
#define RAPROTO_SENSOR_AUTOSAVE				0
#define RAPROTO_SENSOR_AUTOTRANSMIT			1
#define RAPROTO_SENSOR_BATTERY				2
#define RAPROTO_SENSOR_ACC					3
#define RAPROTO_SENSOR_HRM					4
#define RAPROTO_SENSOR_GYRO					5
#define RAPROTO_SENSOR_GYRO_ROTATION			6
#define RAPROTO_SENSOR_HRM_GREEN				7
#define RAPROTO_SENSOR_GRAVITY				8

//TODO: Remove SAVE_RATE from API -- setup to save every minute -- non-configurable
//TODO: Remove Battery from API -- setup to save every minute -- non-configurable
#define QUEUE_SIZE_MESSAGES					10000 // 10k chosen for server side MQTT management -- could be larger.
//#define QUEUE_SIZE_SYSTEM 					120 // 1 Hz (max) * 60 min/sec * 2 for worst case
//#define QUEUE_SIZE_ACCELEROMETER				3600 // 30 Hz (max) * 60 min/sec *  2 for worst case
//#define QUEUE_SIZE_HEART_RATE_MONITOR		120 // 1 Hz (max) * 60 min/sec * 2 for worst case

#define ERROR_MESSAGE						"MSG"

typedef enum {
  RAPROTO_ERROR_NONE = 0,
  RAPROTO_ERROR_PASS	 = 1,
  RAPROTO_ERROR_INVALID_INPUT = 2,
  RAPROTO_ERROR_MESSAGE_SIZE = 10,
  RAPROTO_ERROR_WIFI	 = 11,
  RAPROTO_ERROR_MQTT	 = 20
} Raproto_Error_Type_t;


#define RAPROTO_DATA_STORE_ON 		1
#define RAPROTO_DATA_STORE_OFF		0
#define RAPROTO_DATA_FILENAME		"raproto_data.txt"
#define RAPROTO_DATA_IDX_START		"IDX_START"
#define RAPROTO_DATA_IDX_END			"IDX_END"
#define RAPROTO_DATA_LENGTH			"LENGTH"
#define RAPROTO_DATA_MAX_IDX			2147483645  // two less than a signed int max

#define RAPROTO_TIMEOUT_MONITOR_SECONDS		300 // 5 minutes

#define MAX_FILE_PATH				100



#define RAPROTO_MAX_MESSAGE_SIZE 	1000
#define RAPROTO_MAX_KEY_SIZE 		25
#define RAPROTO_MAX_DISPLAY_SIZE		25

#define RAPROTO_MUTEX_ATTEMPTS		3
#define RAPROTO_MUTEX_SLEEP			100

#define RAPROTO_MONITOR_NAME									"NAME"
#define RAPROTO_MONITOR_NAME_TASK							"TASK"
#define RAPROTO_MONITOR_NAME_LOG								"LOG"
#define	RAPROTO_MONITOR_STATE								"STATE"
#define	RAPROTO_MONITOR_STATE_ON								"ON"
#define RAPROTO_MONITOR_STATE_OFF							"OFF"

#define RAPROTO_SETTING_SERVICE_STATE_ON						"ON"
#define RAPROTO_SETTING_SERVICE_STATE_OFF					"OFF"
#define RAPROTO_SETTING_SERVICE_STATE_LOGGING				"LOGGING"
#define RAPROTO_SETTING_SERVICE_STATE_ERROR					"ERROR"

#define RAPROTO_UPDATE_INITIAL_MESSAGE 						"Press HERE to Update"
#define RAPROTO_UPDATE_INPROGRESS_MESSAGE 					"In Progress ..."
#define RAPROTO_UPDATE_ERROR_WIFI							"ERROR: wifi timeout"
#define RAPROTO_UPDATE_COMPLETE_MESSAGE						"Update Complete"

#define RAPROTO_ERROR_MSG_WIFI								"ERROR: WIFI"
#define RAPROTO_ERROR_MSG_MQTT								"ERROR: MQTT"
#define RAPROTO_ERROR_MSG_MONITOR							"ERROR: MONITOR"
#define RAPROTO_ERROR_MSG_CONFIG								"ERROR: CONFIG"

#define RAPROTO_SYNC_INITIAL_MESSAGE							"Last Sync: "
#define RAPROTO_SYNC_INPROGRESS_MESSAGE 						"In Progress ..."
#define RAPROTO_SYNC_ERROR_RUNNING							"ERROR: sync failure -- try again"

#define RAPROTO_SETTING_ERROR_FALSE					0
#define RAPROTO_SETTING_ERROR_TRUE					1

#define RAPROTO_SETTING_LOGGING_FALSE				0
#define RAPROTO_SETTING_LOGGING_TRUE					1


#include <bundle.h>
#include <dlog.h>
#include <stdlib.h>
#include <stdio.h>
#include <system_info.h>

bundle* settings_init(void);

#endif /* SETTINGS_H_ */
