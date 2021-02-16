#ifndef __raprotoservice_H__
#define __raprotoservice_H__

#include "types.h"
#include "settings.h"
#include "utility.h"
#include "error.h"
#include "data.h"
#include "config.h"
#include "transmit.h"
#include "wifi.h"
#include "mqtt.h"
#include "monitor.h"
#include "task_manager.h"

#define LOGA_DEBUG 0
#define LOGA_INFO 1
//#include <stdarg.h>
//#include <time.h>
//#include <sys/timeb.h>

//#include <utils_i18n.h>

//#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "RaprotoService"

/* Sync Adapter application ID for removing account ID */
#define RAPROTO_APP_ID 							"org.precise.raproto.ui"


#endif /* __raprotoservice_H__ */
