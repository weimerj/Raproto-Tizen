/*
 * wifi.h
 *
 *  Created on: Feb 14, 2021
 *      Author: weimer
 */

#ifndef WIFI_H_
#define WIFI_H_

#include "types.h"
#include "error.h"
#include "settings.h"
#include "app_control.h"
#include "wifiautoconnect.h"
#include "task_manager.h"

void wifi_start(app_data_s *ad);
void wifi_deinitialize(app_data_s *ad);
void wifi_stop(app_data_s *ad);
void wifi_initialize(app_data_s *ad);

#endif /* WIFI_H_ */
