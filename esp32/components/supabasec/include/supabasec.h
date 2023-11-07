#ifndef SUPABASEC_H_
#define SUPABASEC_H_

#include "logs.h"

#include "esp_http_client.h"
#include "esp_log.h"

extern const char ssl_cert_pem_start[] asm("_binary_ssl_cert_pem_start");
extern const char ssl_cert_pem_end[] asm("_binary_ssl_cert_pem_end");

#define HTTP_SERVER_URL "https://wvjiwtbxvehgbdskepdn.supabase.co/rest/v1/test"
#define API_KEY "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyAgCiAgICAicm9sZSI6ICJhbm9uIiwKICAgICJpc3MiOiAic3VwYWJhc2UtZGVtbyIsCiAgICAiaWF0IjogMTY0MTc2OTIwMCwKICAgICJleHAiOiAxNzk5NTM1NjAwCn0.dc_X5iR_VP_qT0zsiyj_I_OZ2T9FtRU2BBNWN8Bu4GE"
#define BEARER_VALUE "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyAgCiAgICAicm9sZSI6ICJhbm9uIiwKICAgICJpc3MiOiAic3VwYWJhc2UtZGVtbyIsCiAgICAiaWF0IjogMTY0MTc2OTIwMCwKICAgICJleHAiOiAxNzk5NTM1NjAwCn0.dc_X5iR_VP_qT0zsiyj_I_OZ2T9FtRU2BBNWN8Bu4GE"

extern esp_http_client_handle_t http_client;
extern esp_http_client_config_t http_cfg;

extern bool connected;
extern struct logs ecu2;
extern int64_t stest;
extern QueueHandle_t supabase_q;
extern SemaphoreHandle_t can_insert;

void supabase_insert();

#endif