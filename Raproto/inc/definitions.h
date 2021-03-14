/*
 * definitions.h
 *
 *  Created on: Feb 15, 2021
 *      Author: weimer
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_


/* Raproto application ID for removing account ID */
#define RAPROTO_APP_ID "org.precise.raproto.ui"


//#define RAPROTO_UPDATE_THEME					"event.org.precise.raproto.ui.update_theme"
#define RAPROTO_LAUNCH_TASK_EVENT			"event.org.precise.raproto.ui.launch_task"
#define RAPROTO_TLAUNCH_HEART_BEAT			"event.org.precise.raproto.service.launch_heart_beat"


#define RAPROTO_TOPIC_SETTINGS				"event.org.precise.raproto.service.publish_settings"
//#define RAPROTO_TOPIC_HEART_BEAT				"event.org.precise.raproto.service.publish_heart_beat"

//#define RAPROTO_TOPIC_CONFIG				"event.org.precise.raproto.service.publish_config"
//#define RAPROTO_TOPIC_STATUS				"event.org.precise.raproto.service.publish_status"
//#define RAPROTO_TOPIC_SYNC					"event.org.precise.raproto.service.publish_sync"

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





/*
 * Package name can coupling the applications,
 * Raproto and Raproto Service
 */
#if !defined(PACKAGE)
#define PACKAGE "org.precise.raproto"
#endif

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "Raproto"


typedef enum {
  RAPROTO_ERROR_NONE = 0,
  RAPROTO_ERROR_INVALID_INPUT = 1
} Raproto_Error_Type_t;


/* The line of genlist */
#define NUM_OF_MAIN_MENU_ITEMS 6
#define MAIN_MENU_DEVICE_ID 0
#define MAIN_MENU_ONOFF 1
#define MAIN_MENU_SYNC 2
#define MAIN_MENU_SETTINGS 3
#define MAIN_MENU_ABOUT 4
#define MAIN_MENU_EXIT 5

#define NUM_OF_SETTINGS_MENU_ITEMS 2
#define SETTINGS_CONFIG 0
#define SETTINGS_MQTT 1

#define NUM_OF_ABOUT_MENU_ITEMS 4
#define ABOUT_VERSION 0
#define ABOUT_INFO 1
#define ABOUT_DISCLAIMER 2
#define ABOUT_HELP 3

#define NUM_OF_MQTT_MENU_ITEMS 3
#define MQTT_BROKER 0
#define MQTT_USERNAME 1
#define MQTT_PASSWORD 2

#define NUM_OF_CONFIG_MENU_ITEMS 3
#define CONFIG_UPDATE 0
#define CONFIG_SUBSCRIBE 1
#define CONFIG_PUBLISH 2

#define MAX_FILE_PATH				100
#define MAX_STRING_SIZE 				100
#define MAX_MARKUP_SIZE				100

/*
 * Raproto Service application ID for registering
 * the Raproto Service as a raproto
 */
#define RAPROTO_SERVICE_APP_ID 					"org.precise.raproto.service"
#define RAPROTO_SERVICE_CONTROL_PORT_ID		 	"raproto.service.control"
#define RAPROTO_SERVICE_CONTROL					"service_action"
#define RAPROTO_SERVICE_CONTROL_START			1
#define RAPROTO_SERVICE_CONTROL_STOP				0
#define RAPROTO_SERVICE_CONTROL_TX_NOW			2

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
#define RAPROTO_SETTING_DATA_STORED				"DATA_STORED"
#define RAPROTO_SETTING_BATTERY					"BATTERY"
#define RAPROTO_SETTING_HEART_BEAT				"HEART_BEAT"
#define RAPROTO_SETTING_LOGGING					"SERVICE_LOGGING"
#define RAPROTO_SETTING_ERROR					"SERVICE_ERROR"

#define RAPROTO_SETTING_SENSOR_ACC				"ACC"
#define RAPROTO_SETTING_SENSOR_HR				"HR"
#define RAPROTO_SETTING_SENSOR_PPG				"PPG"
#define RAPROTO_SETTING_SENSOR_TEMP				"TEMP"
#define RAPROTO_SETTING_SENSOR_SLEEP				"SLEEP"
#define RAPROTO_SETTING_SENSOR_STEPS				"STEPS"
#define RAPROTO_SETTING_SENSOR_STRESS			"STRESS"
#define RAPROTO_SETTING_SENSOR_GYRO 				"GYRO"
#define RAPROTO_SETTING_SENSOR_MAG				"MAG"


#define RAPROTO_TASK_REQUEST								"TASK_REQUEST"
#define RAPROTO_TASK_REQUEST_START						"START_SERVICE"
#define RAPROTO_TASK_REQUEST_STOP						"STOP_SERVICE"
#define RAPROTO_TASK_REQUEST_STOP_SOFT					"SOFT_STOP_SERVICE"
#define RAPROTO_TASK_REQUEST_CLEAR_QUEUE					"CLEAR_QUEUE"
#define RAPROTO_TASK_REQUEST_SETTINGS_UPDATE				"SETTINGS_UPDATE"
#define RAPROTO_TASK_REQUEST_TRANSMIT					"TRANSMIT_DATA"
#define RAPROTO_TASK_REQUEST_LOG_ON						"LOG_ON"
#define RAPROTO_TASK_REQUEST_LOG_OFF						"LOG_OFF"
#define RAPROTO_TASK_REQUEST_LOG_CHECK					"LOG_CHECK"
#define RAPROTO_TASK_REQUEST_SETTINGS_TRANSMIT		    	"SETTINGS_TRANSMIT"
#define RAPROTO_TASK_REQUEST_ERROR						"ERROR"
#define RAPROTO_TASK_REQUEST_FACTORY_RESET				"FACTORY_RESET"



#define RAPROTO_SETTING_SERVICE_STATE_ON						"ON"
#define RAPROTO_SETTING_SERVICE_STATE_OFF					"OFF"
#define RAPROTO_SETTING_SERVICE_STATE_LOGGING				"LOGGING"
#define RAPROTO_SETTING_SERVICE_STATE_ERROR					"ERROR"

#define RAPROTO_SETTING_FILENAME								"raproto_config.txt"

#define RAPROTO_MONITOR_NAME									"NAME"
#define RAPROTO_MONITOR_NAME_TASK							"TASK"
#define RAPROTO_MONITOR_NAME_LOG								"LOG"
#define	RAPROTO_MONITOR_STATE								"STATE"
#define	RAPROTO_MONITOR_STATE_ON								"ON"
#define RAPROTO_MONITOR_STATE_OFF							"OFF"

#define RAPROTO_STATE_ERROR							-1
#define RAPROTO_STATE_CHECK							0
#define RAPROTO_STATE_OK								1

#define RAPROTO_SETTING_ERROR_FALSE					0
#define RAPROTO_SETTING_ERROR_TRUE					1

#define RAPROTO_SETTING_LOGGING_FALSE				0
#define RAPROTO_SETTING_LOGGING_TRUE					1
#define RAPROTO_SETTING_LOGGING_DONE					2

#define RAPROTO_SLEEP_START							100


#define RAPROTO_COLOR_BACKGROUND_DEFAULT							  0,   0,   0
#define RAPROTO_COLOR_BACKGROUND_LOGGING							  0, 100,   0
#define RAPROTO_COLOR_BACKGROUND_ERROR							100,   0,   0
#define RAPROTO_COLOR_GENLIST_DEFAULT							  0,   0,   0
#define RAPROTO_COLOR_GENLIST_LOGGING							  0,  80,   0
#define RAPROTO_COLOR_GENLIST_ERROR								 80,   0,   0

#define RAPROTO_HEART_BEAT										1
#define RAPROTO_ERROR_RESET_TAPS									5
#define RAPROTO_MUTEX_SLEEP										100

static const char *font_markup_main = \
  "<font=Tizen font_style=normal font_width=normal font_size=40 color=#FFFFFF>";

static const char *font_markup_sub = \
  "<font=Tizen font_style=normal font_width=normal font_size=25 color=#C0C0C0>";


static const char *disclaimer_message = \
  "<align=center>DISCLAIMER: USE OF THIS SOFTWARE IN ANY WAY AND " \
  "RELIANCE ON THE CONTENTS IS DONE AT THE USER'S DISCRETION AND RISK. " \
  "YOU HEREBY AGREE THAT YOU WILL BE SOLELY RESPONSIBLE FOR ANY DAMAGE " \
  "OR HARM THAT RESULTS FROM SUCH USE. THIS SOFTWARE IS PROVIDED BY THE " \
  "COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR " \
  "IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED " \
  "WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE " \
  "ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS " \
  "BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, " \
  "OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT " \
  "OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR "\
  "BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, "\
  "WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE "\
  "OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN "\
  "IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.</align>";


static const char *help_message = \
  "<align=center>TO START LOGGING:<br>"
  "step 1:<br>configure system (if needed)<br>"
  "step 2:<br>Press on<br>" \
  "<br>TO CONFIGURE:<br>" \
  "*** Requires Wifi ***<br>" \
  "Settings->Configuration->Update<br>" \
  "If successful, \"Update Complete\" will be displayed below \"Update\"</align>";


static const char *info_message = \
	"<align=center>Raproto: A rapid prototyping platform for the Internet-of-Medical-Things. " \
	"<br><br>Developed by: The PRECISE Center, University of Pennsylvania<br><br>" \
	" Raproto provides data collection, storage, analysis APIs, and feedback mechanisms " \
	"that can be utilized to prototype various medical devices using inexpensive hardware" \
	" commonly available."\
	"<br><br>For more information about future developments, contact: <br>" \
	"James Weimer <br> weimerj@seas.upenn.edu</align>";








#endif /* DEFINITIONS_H_ */
