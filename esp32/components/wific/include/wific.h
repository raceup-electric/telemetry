#ifndef WIFIC_H_
#define WIFIC_H_

#include "esp_bit_defs.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

#define WIFI_SSID           "."
#define WIFI_PASSWORD       "8caratteri"
#define WIFI_MAXIMUM_RETRY  10
#define WIFI_CONNECTED_BIT  BIT0
#define WIFI_FAIL_BIT       BIT1

bool wifi_init();

#endif