#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_timer.h"

// COMPONENTS
#include "logs.h"
#include "wific.h"
#include "serialc.h"
#include "cobs.h"
#include "supabasec.h"

// EXTERN DEFINITIONS
bool connected = false;
esp_http_client_handle_t http_client;
esp_http_client_config_t http_cfg = {
    .url = HTTP_SERVER_URL,
    .timeout_ms = 5000,
    .buffer_size = 1024,
    .buffer_size_tx = 2048,
    .method = HTTP_METHOD_POST,
    .transport_type = HTTP_TRANSPORT_OVER_SSL,
    .cert_pem = ssl_cert_pem_start};

// CPU
struct logs ecu;
struct logs ecu2;
QueueHandle_t supabase_q;
SemaphoreHandle_t can_insert;

void app_main(void)
{
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  connected = wifi_init();
  serial_init();

  supabase_q = xQueueCreate(1, sizeof(struct logs));
  can_insert = xSemaphoreCreateBinary();
  xSemaphoreGive(can_insert);

  http_client = esp_http_client_init(&http_cfg);
  esp_http_client_set_header(http_client, "apikey", API_KEY);
  esp_http_client_set_header(http_client, "Authorization", BEARER_VALUE);
  esp_http_client_set_header(http_client, "Content-Type", "application/json");
  esp_http_client_set_header(http_client, "Prefer", "return=minimal");

  xTaskCreatePinnedToCore(serial_receive, "serial_receive", BUF_SIZE * 2, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(supabase_insert, "supabase_insert", 65536, NULL, tskIDLE_PRIORITY, NULL, 1);
}