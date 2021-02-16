/*
 * monitor.h
 *
 *  Created on: Feb 14, 2021
 *      Author: weimer
 */

#ifndef MONITOR_H_
#define MONITOR_H_

#include "settings.h"
#include "error.h"
#include "types.h"
#include "task_manager.h"



void monitor_control(int sensor, int period, const char *state, app_data_s *ad);
bool monitor_heart_beat(int idx, app_data_s *ad);
void monitor_init(app_data_s *ad);
bool monitor_cb(app_data_s *ad);

#endif /* MONITOR_H_ */
