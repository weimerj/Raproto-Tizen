/*
 * settings_menu.h
 *
 *  Created on: Feb 15, 2021
 *      Author: weimer
 */

#ifndef SETTINGS_MENU_H_
#define SETTINGS_MENU_H_

#include "types.h"
#include "definitions.h"
#include "utility.h"
#include <dlog.h>
#include "config_menu.h"
#include "mqtt_menu.h"
#include "main_menu.h"

//#include <app_event.h>
//#include <app_common.h>
//#include "settings_menu.h"
//#include "about_menu.h"
//#include "launch_service.h"

void update_settings(const char *event_name, bundle *settings, void *data);
void create_settings_menu(app_data_s *ad);


#endif /* SETTINGS_MENU_H_ */
