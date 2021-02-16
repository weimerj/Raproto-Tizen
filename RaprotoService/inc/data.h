/*
 * data.h
 *
 *  Created on: Feb 14, 2021
 *      Author: weimer
 */

#ifndef DATA_H_
#define DATA_H_

#include <stdlib.h>
#include <stdio.h>
#include <app_common.h>
#include <app_control.h>
#include "settings.h"
#include "log.h"
#include "types.h"
#include "error.h"



void data_save(bundle *data, const char *filename);
Eina_Bool data_autosave(void *data);
void data_load_user(char *key, app_control_h app, app_data_s *ad) ;
bundle* data_load_settings(void);
bundle* data_load_data(void);
void data_setup(app_data_s *ad);


#endif /* DATA_H_ */
