/*
 * launch_service.h
 *
 *  Created on: Feb 15, 2021
 *      Author: weimer
 */

#ifndef LAUNCH_SERVICE_H_
#define LAUNCH_SERVICE_H_

#include "types.h"
#include "definitions.h"
#include "utility.h"
#include <app_manager.h>

void launch_service_start(app_data_s *ad);
void launch_service_task(char *task, app_data_s *ad);
void launch_service_task_event(const char *event_name, bundle *task_b, void *data);

#endif /* LAUNCH_SERVICE_H_ */
