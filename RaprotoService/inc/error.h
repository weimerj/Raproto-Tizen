/*
 * error.h
 *
 *  Created on: Feb 13, 2021
 *      Author: weimer
 */

#ifndef ERROR_H_
#define ERROR_H_

#include "settings.h"
#include <app_event.h>
#include <service_app.h>
#include <bundle.h>
#include <dlog.h>
#include "types.h"


void error_msg(int rc, char const *caller_name, char const *id);
void	 warn_msg(int rc, char const *caller_name, char const *id);
void error_msg_event(const char *event_name, bundle *junk, void *data);

#endif /* ERROR_H_ */
