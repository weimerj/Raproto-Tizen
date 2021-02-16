/*
 * log.h
 *
 *  Created on: Feb 14, 2021
 *      Author: weimer
 */

#ifndef LOG_H_
#define LOG_H_

#include "settings.h"
#include "types.h"
#include "error.h"
#include "utility.h"
#include "monitor.h"
#include "transmit.h"
#include "data.h"
#include "task_manager.h"
#include <device/power.h>
#include <unistd.h>
#include <device/battery.h>
#include <device/callback.h>

void log_stop(app_data_s *ad);
void log_start(app_data_s *ad);
void log_message_pack_all(app_data_s *ad, int attempt);

#endif /* LOG_H_ */
