#ifndef WIFIC_H_
#define WIFIC_H_

// esp32 libraries
#include "esp_bit_defs.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

// global definition
#include "globals.h"

#define WIFI_CONNECTED_BIT  BIT0
#define WIFI_FAIL_BIT       BIT1

bool wifi_init();

#endif