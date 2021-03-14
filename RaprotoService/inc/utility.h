/*
 * utility.h
 *
 *  Created on: Feb 14, 2021
 *      Author: weimer
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include "settings.h"
#include "error.h"
#include <sys/time.h>
#include <time.h>


void refresh_display(bundle *settings);
void utility_log_amount_of_data(app_data_s *ad);
int queue_size(int start_idx, int end_idx, int queue_size);
void increment_queue_idx(int* idx, int N, int queue_size);
void utility_time_update(bundle *settings, char *key, const char *format);

#endif /* UTILITY_H_ */
