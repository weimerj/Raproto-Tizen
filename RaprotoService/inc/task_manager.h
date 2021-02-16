/*
 * task_manager.h
 *
 *  Created on: Feb 14, 2021
 *      Author: weimer
 */

#ifndef TASK_MANAGER_H_
#define TASK_MANAGER_H_

#define JSMN_HEADER

#include "settings.h"
#include "types.h"
#include "error.h"

#include "wifi.h"
#include "config.h"
#include "mqtt.h"
#include "log.h"

void task_start(app_data_s *ad);
void task_warn(const char *msg, app_data_s *ad);
void task_error(const char *msg, app_data_s *ad);
void task_process_cb(const char* event);
void task_process(const char *event_name, bundle *junk, void *data);

#endif /* TASK_MANAGER_H_ */
