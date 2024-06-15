#include <stdio.h>
#include "wific.h"

#include "freertos/event_groups.h"

static int s_retry_num = 0;
static EventGroupHandle_t s_wifi_event_group;

static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT)
    {
        if (s_retry_num >= WIFI_MAXIMUM_RETRY)
        {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
            return;
        }
        switch (event_id)
        {
        case WIFI_EVENT_STA_DISCONNECTED:
            s_retry_num++;
            break;
        case WIFI_EVENT_STA_START:
            esp_wifi_connect();
            break;
        default:
            break;
        }
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

bool wifi_init(void)
{
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, NULL));


    ESP_ERROR_CHECK(esp_netif_init());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD},
    };
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI("WiFi", "Connected to AP SSID:%s, password:%s", WIFI_SSID, WIFI_PASSWORD);
        return true;
    }
    else if (bits & WIFI_FAIL_BIT)
    {
        ESP_LOGI("WiFi", "Failed to connect to SSID:%s, password:%s", WIFI_SSID, WIFI_PASSWORD);
        return false;
    }
    else
    {
        ESP_LOGE("WiFi", "UNEXPECTED EVENT");
        return false;
    }
}
