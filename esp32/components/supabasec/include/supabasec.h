#ifndef SUPABASEC_H_
#define SUPABASEC_H_

// import components
#include "structures.h"
#include "globals.h"

// import esp32 libraries
#include "esp_http_client.h"
#include "esp_log.h"
// VERY IMPORTANT: edit 'zconf.h' line 263 'MAX_MEM_LEVEL' and set it to 7
#include "zlib.h"

// import ssl certificate for supabase connection
extern const char ssl_cert_pem_start[] asm("_binary_ssl_cert_pem_start");
extern const char ssl_cert_pem_end[] asm("_binary_ssl_cert_pem_end");

extern esp_http_client_handle_t http_client;
extern esp_http_client_config_t http_cfg;

extern bool connected;
extern struct logs ecu2;                    // store copy of ecu struct from queue
extern int64_t stest;                       // test identifier
extern QueueHandle_t supabase_q;            // received new struct on uart
extern SemaphoreHandle_t can_insert;        // already using ecu2?

void supabase_insert();

#endif