/*
 * mqtt_menu.h
 *
 *  Created on: Feb 15, 2021
 *      Author: weimer
 */

#ifndef MQTT_MENU_H_
#define MQTT_MENU_H_

#include "types.h"
#include "definitions.h"
#include "utility.h"
#include "launch_service.h"

void return_publish_topic(void *data, Evas_Object *entry, void *event_info);
void return_subscribe_topic(void *data, Evas_Object *entry, void *event_info);
void create_mqtt_menu(app_data_s *ad);

#endif /* MQTT_MENU_H_ */
