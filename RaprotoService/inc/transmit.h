/*
 * transmit.h
 *
 *  Created on: Feb 14, 2021
 *      Author: weimer
 */

#ifndef TRANSMIT_H_
#define TRANSMIT_H_


#include "settings.h"
#include "types.h"
#include "error.h"
#include "utility.h"
#include "config.h"
#include "task_manager.h"

void transmit_data(app_data_s *ad);
Eina_Bool transmit_autorequest(void *data);
void transmit_start(app_data_s *ad);
void transmit_data_setup(app_data_s *ad);

#endif /* TRANSMIT_H_ */
