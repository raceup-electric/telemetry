// esp32 libraries
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_timer.h"

// COMPONENTS
#include "zlib.h"
#include "structures.h"
#include "globals.h"
#include "wific.h"
#include "serialc.h"
#include "cobs.h"
#include "supabasec.h"

// EXTERN DEFINITIONS
bool connected = false;                             // connected to wifi?
nvs_handle_t nsv_stest_handle;                      // Non Volatile Storage for stest
esp_http_client_handle_t http_client;               // HTTP client for POST request
esp_http_client_config_t http_cfg = {               // HTTP client configuration
    .url = HTTP_SERVER_URL,
    .timeout_ms = 5000,
    .buffer_size = 1024,
    .buffer_size_tx = 2048,
    .method = HTTP_METHOD_POST,
    .transport_type = HTTP_TRANSPORT_OVER_SSL,
    .cert_pem = ssl_cert_pem_start
};

// DEFINItioNS
struct logs ecu;                                    // Store struct from UART
struct logs ecu2;                                   // Struct to read for db insert
int64_t stest = -1;                                 // Default test init number (-1 because incremented on main)
QueueHandle_t supabase_q;
SemaphoreHandle_t can_insert;

void app_main(void)
{
    // Non Volatile Storage setup
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    nvs_open("storage", NVS_READWRITE, &nsv_stest_handle);

    // Get current test number
    nvs_get_i64(nsv_stest_handle, "stest", &stest);
    stest++;
    nvs_set_i64(nsv_stest_handle, "stest", stest);

    // Init uart and wifi
    connected = wifi_init();
    serial_init();

    // Init queue
    supabase_q = xQueueCreate(1, sizeof(struct logs));
    can_insert = xSemaphoreCreateBinary();
    xSemaphoreGive(can_insert);

    // Init http client
    http_client = esp_http_client_init(&http_cfg);
    //esp_http_client_set_header(http_client, "apikey", API_KEY);
    //esp_http_client_set_header(http_client, "Authorization", BEARER_VALUE);
    esp_http_client_set_header(http_client, "Content-Type", "application/json");
    //esp_http_client_set_header(http_client, "Prefer", "return=minimal");
    esp_http_client_set_header(http_client, "Content-Encoding", "gzip");

    // Set tasks to cores
    xTaskCreatePinnedToCore(serial_receive, "serial_receive", BUF_SIZE * 2, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(supabase_insert, "supabase_insert", 4096, NULL, tskIDLE_PRIORITY, NULL, 1);
}