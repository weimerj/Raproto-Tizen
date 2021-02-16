/*
 * utility.c
 *
 *  Created on: Feb 14, 2021
 *      Author: weimer
 */


#include "utility.h"


void
increment_queue_idx(int* idx, int N, int queue_size){
	*idx = (*idx + N) % queue_size;
	return;
}



void
utility_time_update(bundle *settings, char *key, const char *format){

	struct timeval tv;
	time_t nowtime;
	struct tm *nowtm;

	//tv = time(NULL);
	gettimeofday(&tv, NULL); //TODO: fix, it is depricated
	nowtime = tv.tv_sec;
	nowtm = localtime(&nowtime);
	char update[RAPROTO_MAX_DISPLAY_SIZE];

	strftime(update, sizeof(update), format, nowtm);
	bundle_del(settings, key);
	bundle_add_str(settings, key, update);
}
