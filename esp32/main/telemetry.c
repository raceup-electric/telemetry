#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_timer.h"

// WiFi CONFIGURATION
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#define WIFI_SSID "Velex_Raceup"
#define WIFI_PASSWORD "Velex.2021"
#define WIFI_MAXIMUM_RETRY 10
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1
static int s_retry_num = 0;
static EventGroupHandle_t s_wifi_event_group;

// UART CONFIGURATION
#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/queue.h"
#define UART_TX GPIO_NUM_10
#define UART_RX GPIO_NUM_9
#define UART_RTS UART_PIN_NO_CHANGE
#define UART_CTS UART_PIN_NO_CHANGE
#define UART_NUM UART_NUM_1
#define UART_BAUD 115200
#define BUF_SIZE (1024)
static QueueHandle_t uart_queue;

// SUPABASE CONFIGURATION
#include "esp_http_client.h"
#define HTTP_SERVER_URL "https://wvjiwtbxvehgbdskepdn.supabase.co/rest/v1/test"
#define API_KEY "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Ind2aml3dGJ4dmVoZ2Jkc2tlcGRuIiwicm9sZSI6ImFub24iLCJpYXQiOjE2OTM4MjEzNzMsImV4cCI6MjAwOTM5NzM3M30.-QJcyNwn_4Dtkvohg-g3GJPb0jCoePxB1MEH7tzFbbo"
#define BEARER_VALUE "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Ind2aml3dGJ4dmVoZ2Jkc2tlcGRuIiwicm9sZSI6ImFub24iLCJpYXQiOjE2OTM4MjEzNzMsImV4cCI6MjAwOTM5NzM3M30.-QJcyNwn_4Dtkvohg-g3GJPb0jCoePxB1MEH7tzFbbo"
extern const char ssl_cert_pem_start[] asm("_binary_ssl_cert_pem_start");
extern const char ssl_cert_pem_end[] asm("_binary_ssl_cert_pem_end");

// LOGS CONFIGURATION
struct logs
{
  int64_t local_timestamp;
} ecu;

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

void wifi_init_sta(void)
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
    ESP_LOGI("WiFi", "Connected to AP SSID:%s, password:%s", WIFI_SSID, WIFI_PASSWORD);
  else if (bits & WIFI_FAIL_BIT)
    ESP_LOGI("WiFi", "Failed to connect to SSID:%s, password:%s", WIFI_SSID, WIFI_PASSWORD);
  else
    ESP_LOGE("WiFi", "UNEXPECTED EVENT");
}

void serial_init(void)
{
  uart_config_t uart_config = {
      .baud_rate = UART_BAUD,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
      .source_clk = UART_SCLK_DEFAULT,
  };
  int intr_alloc_flags = 0;

  ESP_ERROR_CHECK(uart_driver_install(UART_NUM, BUF_SIZE, BUF_SIZE, 10, &uart_queue, intr_alloc_flags));
  ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_config));
  ESP_ERROR_CHECK(uart_set_pin(UART_NUM, UART_TX, UART_RX, UART_RTS, UART_CTS));
  ESP_ERROR_CHECK(uart_enable_pattern_det_baud_intr(UART_NUM, '\0', 1, 10, 0, 0));
  ESP_ERROR_CHECK(uart_pattern_queue_reset(UART_NUM, 10));
}

void serial_send(char *_struct)
{
  uart_write_bytes(UART_NUM, (const char *)_struct, strlen(_struct));
}

void serial_receive(int packet_len)
{
  uint8_t *data = (uint8_t *)malloc(BUF_SIZE);

  uart_event_t uart_event;
  size_t buffered_size = 0;
  while (1)
  {
    if (xQueueReceive(uart_queue, (void *)&uart_event, pdMS_TO_TICKS(10)))
    {
      switch (uart_event.type)
      {
      case UART_PATTERN_DET:
      {
        uart_get_buffered_data_len(UART_NUM, &buffered_size);
        int pattern_pos = uart_pattern_pop_pos(UART_NUM);
        int read_len = uart_read_bytes(UART_NUM, data, pattern_pos, pdMS_TO_TICKS(10));
        uart_flush_input(UART_NUM);
        data[read_len] = '\0';
        ESP_LOGI("UART", "Received string: %s", data);
        memset(data, 0, BUF_SIZE);
      }
      default:
        break;
      }
    }
  }
}

void db_insert(void)
{
  esp_http_client_config_t http_cfg = {
      .url = HTTP_SERVER_URL,
      .timeout_ms = 5000,
      .buffer_size = 1024,
      .buffer_size_tx = 2048,
      .method = HTTP_METHOD_POST,
      .transport_type = HTTP_TRANSPORT_OVER_SSL,
      .cert_pem = ssl_cert_pem_start};
  esp_http_client_handle_t http_client = esp_http_client_init(&http_cfg);
  esp_http_client_set_header(http_client, "apikey", API_KEY);
  esp_http_client_set_header(http_client, "Authorization", BEARER_VALUE);
  esp_http_client_set_header(http_client, "Content-Type", "application/json");
  esp_http_client_set_header(http_client, "Prefer", "return=minimal");
  char body[512];

  int body_len = sprintf(body, "\
    {\"local_timestamp\": \"%llu\"\
  }", ecu.local_timestamp);

  esp_http_client_set_post_field(http_client, body, body_len);
  esp_http_client_perform(http_client);
  esp_http_client_cleanup(http_client);
}

void app_main(void)
{
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  wifi_init_sta();
  serial_init();

  //serial_receive(16);

  while(1) {
    ecu.local_timestamp = esp_timer_get_time();
    db_insert();
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}