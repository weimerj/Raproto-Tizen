/*
 * config.h
 *
 *  Created on: Feb 14, 2021
 *      Author: weimer
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "settings.h"
#include "types.h"
#include "error.h"
#include "utility.h"
#include "data.h"
#include "task_manager.h"


void config_publish(bundle *settings);
void config_received(app_data_s *ad,  char *payload, int payloadLen);
void config_start(app_data_s *ad);
void config_status(char *val, bundle *settings);


#endif /* CONFIG_H_ */
