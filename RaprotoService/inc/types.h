/*
 * types.h
 *
 *  Created on: Feb 14, 2021
 *      Author: weimer
 */

#ifndef TYPES_H_
#define TYPES_H_

#include <tizen.h>
#include "settings.h"
#include "wifiautoconnect.h"
#include "pahomqtt/MQTTAsync.h"
#include <sensor.h>



typedef struct queue_state{
	char *name;
	int idx_start;
	int idx_end;
	int len;
} queue_state_s;


typedef int (*mqtt_payload_create)(void *, char *, int);


typedef struct mqtt_message_data{
	mqtt_payload_create msg_cb;
	queue_state_s *state;
	int idx_start;
	int idx_end;
} mqtt_message_data_s;


typedef struct mqtt_context{
	MQTTAsync *client;
	int cur_sensor;
	mqtt_message_data_s message_data[RAPROTO_SENSOR_MAX_NUM+1];
	char *payload;
	int payloadlen;
} mqtt_context_s;


typedef struct task_scheduler{
	int tasks[RAPROTO_TASK_NUMBER];
	int active;
} task_scheduler_s;

typedef struct message_data{
	bundle *messages;
	char *payload;
	int payloadlen;
	int payloadmax;
	int mutex;
	bool initialized;
} message_data_s;

typedef struct monitor_data{
	time_t t;
	bool on;
	int period;
} monitor_data_s;

typedef struct sensor_data{
	monitor_data_s monitor;
	sensor_h sensor;
	sensor_listener_h listener;
} sensor_data_s;


// application data (context) that will be passed to functions when needed
typedef struct app_data {
    wifi_manager_h *wifi; // wifi manager
    wifi_autoconnect_s wifi_ac; // autoconnect configuration
    mqtt_context_s mqtt; // mqtt data
    bundle *settings; // service application data
    message_data_s data; // stored sensor data;
    task_scheduler_s scheduler; // scheduler for tasks
    sensor_data_s sensors[RAPROTO_SENSOR_MAX_NUM]; // sensors and monitors
    bool reset;
    bool logging;
    bool display;
} app_data_s;





#endif /* TYPES_H_ */
