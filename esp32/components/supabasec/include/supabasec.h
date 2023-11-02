#ifndef SUPABASEC_H_
#define SUPABASEC_H_

#include "logs.h"

#include "esp_http_client.h"
#include "esp_log.h"

extern const char ssl_cert_pem_start[] asm("_binary_ssl_cert_pem_start");
extern const char ssl_cert_pem_end[] asm("_binary_ssl_cert_pem_end");

#define HTTP_SERVER_URL "https://wvjiwtbxvehgbdskepdn.supabase.co/rest/v1/test"
#define API_KEY "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Ind2aml3dGJ4dmVoZ2Jkc2tlcGRuIiwicm9sZSI6ImFub24iLCJpYXQiOjE2OTM4MjEzNzMsImV4cCI6MjAwOTM5NzM3M30.-QJcyNwn_4Dtkvohg-g3GJPb0jCoePxB1MEH7tzFbbo"
#define BEARER_VALUE "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Ind2aml3dGJ4dmVoZ2Jkc2tlcGRuIiwicm9sZSI6ImFub24iLCJpYXQiOjE2OTM4MjEzNzMsImV4cCI6MjAwOTM5NzM3M30.-QJcyNwn_4Dtkvohg-g3GJPb0jCoePxB1MEH7tzFbbo"

extern esp_http_client_handle_t http_client;
extern esp_http_client_config_t http_cfg;

extern bool connected;
extern struct logs ecu2;
extern QueueHandle_t supabase_q;
extern SemaphoreHandle_t can_insert;

void supabase_insert();

#endif