// esp32 libraries
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_timer.h"
#include "esp_netif_sntp.h"

// COMPONENTS
#include "zlib.h"
#include "structures.h"
#include "globals.h"
#include "wific.h"
#include "serialc.h"
#include "cobs.h"
#include "databasec.h"

// EXTERN DEFINITIONS
bool connected = false;                             // connected to wifi?
nvs_handle_t nsv_stest_handle;                      // Non Volatile Storage for stest
// DEFINITIONS
int64_t stest = -1;                                 // Default test init number (-1 because incremented on main)
QueueHandle_t ecu_data;

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
    ecu_data = xQueueCreate(5, sizeof(struct logs));

    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");
    esp_netif_sntp_init(&config); 
    if (esp_netif_sntp_sync_wait(pdMS_TO_TICKS(10000)) != ESP_OK) {
        ESP_LOGE("SNTP", "Failed to update system time within 10s timeout");
    }
    
    // Set tasks to cores
    xTaskCreatePinnedToCore(serial_receive, "serial_receive", 6144, NULL, tskIDLE_PRIORITY + 1, NULL, 0);
    xTaskCreatePinnedToCore(database_insert, "database_insert", 8192, NULL, tskIDLE_PRIORITY + 1, NULL, 1);
}