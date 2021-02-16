/*
 * main_menu.h
 *
 *  Created on: Feb 15, 2021
 *      Author: weimer
 */

#ifndef MAIN_MENU_H_
#define MAIN_MENU_H_


#include "types.h"
#include "definitions.h"
#include "utility.h"
#include <app_event.h>
#include <app_common.h>
#include "settings_menu.h"
#include "about_menu.h"
#include "launch_service.h"

void create_main_menu(app_data_s *ad);
void create_base_gui(app_data_s *ad);

#endif /* MAIN_MENU_H_ */
