#ifndef _WIFIAUTOCONNECT_H_
#define _WIFIAUTOCONNECT_H_

/**
 * This header file is included to define _EXPORT_.
 */
#include <stdbool.h>
#include <tizen.h>
#include <wifi-manager.h>
#include <stdlib.h>
#include <string.h>
#include <Ecore.h>


#ifdef __cplusplus
extern "C" {
#endif

#define WIFI_AUTOCONNECT_PERIOD			1

typedef void(*wifi_autoconnect_error_cb)(wifi_manager_error_e rc, void *user_data);
typedef void(*wifi_autoconnect_connect_cb)(wifi_manager_connection_state_e state, void *user_data);

typedef struct wifi_autoconnect_settings{
	double timeout;
	time_t start_time;
} wifi_autoconnect_settings_s;

typedef struct wifi_autoconnect_config{
	wifi_manager_config_h config;
	struct wifi_autoconnect_config *next;
}wifi_autoconnect_config_s;

typedef struct wifi_autoconnect{
	wifi_manager_h *wifi;
	wifi_autoconnect_config_s *wifi_config_head;
	wifi_autoconnect_settings_s settings;
	wifi_autoconnect_connect_cb connect_cb;
	wifi_autoconnect_error_cb error_cb;
	void *user_data;
} wifi_autoconnect_s;




void wifi_autoconnect_create(wifi_autoconnect_s *ac, wifi_manager_h *wifi, wifi_autoconnect_connect_cb connected_cb, wifi_autoconnect_error_cb error_cb, void *user_data);
void wifi_autoconnect_start(wifi_autoconnect_s *ac, double timeout);
void wifi_autoconnect_destroy(wifi_autoconnect_s *ac);

#ifdef __cplusplus
}
#endif
#endif // _WIFIAUTOCONNECT_H_

