/*
 * mqtt.h
 *
 *  Created on: Feb 14, 2021
 *      Author: weimer
 */

#ifndef MQTT_H_
#define MQTT_H_

#include "types.h"
#include "settings.h"
#include "error.h"
#include "config.h"
#include "wifi.h"
#include "task_manager.h"

void mqtt_start(app_data_s *ad);
void mqtt_destroy(app_data_s *ad);
void mqtt_stop(app_data_s *ad);


#endif /* MQTT_H_ */
