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
#define WIFI_SSID "."
#define WIFI_PASSWORD "8caratteri"
bool connected = false;
#define WIFI_MAXIMUM_RETRY 10
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1
static int s_retry_num = 0;
static EventGroupHandle_t s_wifi_event_group;

// SUPABASE CONFIGURATION
#include "esp_http_client.h"
#define HTTP_SERVER_URL "https://wvjiwtbxvehgbdskepdn.supabase.co/rest/v1/test"
#define API_KEY "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Ind2aml3dGJ4dmVoZ2Jkc2tlcGRuIiwicm9sZSI6ImFub24iLCJpYXQiOjE2OTM4MjEzNzMsImV4cCI6MjAwOTM5NzM3M30.-QJcyNwn_4Dtkvohg-g3GJPb0jCoePxB1MEH7tzFbbo"
#define BEARER_VALUE "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Ind2aml3dGJ4dmVoZ2Jkc2tlcGRuIiwicm9sZSI6ImFub24iLCJpYXQiOjE2OTM4MjEzNzMsImV4cCI6MjAwOTM5NzM3M30.-QJcyNwn_4Dtkvohg-g3GJPb0jCoePxB1MEH7tzFbbo"
extern const char ssl_cert_pem_start[] asm("_binary_ssl_cert_pem_start");
extern const char ssl_cert_pem_end[] asm("_binary_ssl_cert_pem_end");
esp_http_client_handle_t http_client;
esp_http_client_config_t http_cfg = {
    .url = HTTP_SERVER_URL,
    .timeout_ms = 5000,
    .buffer_size = 1024,
    .buffer_size_tx = 2048,
    .method = HTTP_METHOD_POST,
    .transport_type = HTTP_TRANSPORT_OVER_SSL,
    .cert_pem = ssl_cert_pem_start};

// UART CONFIGURATION
#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/queue.h"
#define UART_TX GPIO_NUM_17
#define UART_RX GPIO_NUM_16
#define UART_RTS UART_PIN_NO_CHANGE
#define UART_CTS UART_PIN_NO_CHANGE
#define UART_NUM UART_NUM_2
#define UART_BAUD 115200
#define BUF_SIZE (1024)
static QueueHandle_t uart_queue;

// COBS CONFIGURATION
#include "cobs.h"

// LOGS CONFIGURATION
#include "logs.h"

// CPU CONFIGURATION
struct logs ecu;
struct logs ecu2;
QueueHandle_t supabase_q;
SemaphoreHandle_t can_insert;

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
  {
    ESP_LOGI("WiFi", "Connected to AP SSID:%s, password:%s", WIFI_SSID, WIFI_PASSWORD);
    connected = true;
  }
  else if (bits & WIFI_FAIL_BIT)
  {
    ESP_LOGI("WiFi", "Failed to connect to SSID:%s, password:%s", WIFI_SSID, WIFI_PASSWORD);
    connected = false;
  }
  else
  {
    ESP_LOGE("WiFi", "UNEXPECTED EVENT");
    connected = false;
  }
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
  ESP_ERROR_CHECK(uart_enable_pattern_det_baud_intr(UART_NUM, '\0', 1, 10, 100, 0));
  ESP_ERROR_CHECK(uart_pattern_queue_reset(UART_NUM, 2));
}

void supabase_insert()
{
  char *body = (char *)malloc(24500);
  while (1)
  {
    if (connected && body != NULL && xQueueReceive(supabase_q, &ecu2, (TickType_t)10) == pdPASS)
    {
      if (xSemaphoreTake(can_insert, 1) == pdTRUE)
      {
        // Controllare decimali
        int body_len = sprintf(body, "{\
              \"bms_lv0\": \"%.2f\", \
              \"bms_lv1\": \"%.2f\", \
              \"bms_lv2\": \"%.2f\", \
              \"bms_lv3\": \"%.2f\", \
              \"bms_lv4\": \"%.2f\", \
              \"bms_lv5\": \"%.2f\", \
              \"bms_lv6\": \"%.2f\", \
              \"bms_lv7\": \"%.2f\", \
              \
              \"amk_status_fl\": \"%i\", \
              \"amk_actual_velocity_fl\": \"%.2f\", \
              \"amk_torque_current_fl\": \"%.2f\", \
              \"amk_voltage_fl\": \"%i\", \
              \"amk_current_fl\": \"%.2f\", \
              \
              \"amk_status_fr\": \"%i\", \
              \"amk_actual_velocity_fr\": \"%.2f\", \
              \"amk_torque_current_fr\": \"%.2f\", \
              \"amk_voltage_fr\": \"%i\", \
              \"amk_current_fr\": \"%.2f\", \
              \
              \"amk_status_rr\": \"%i\", \
              \"amk_actual_velocity_rr\": \"%.2f\", \
              \"amk_torque_current_rr\": \"%.2f\", \
              \"amk_voltage_rr\": \"%i\", \
              \"amk_current_rr\": \"%.2f\", \
              \
              \"amk_status_rl\": \"%i\", \
              \"amk_actual_velocity_rl\": \"%.2f\", \
              \"amk_torque_current_rl\": \"%.2f\", \
              \"amk_voltage_rl\": \"%i\", \
              \"amk_current_rl\": \"%.2f\", \
              \
              \"amk_temp_motor_fl\": \"%.2f\", \
              \"amk_temp_inverter_fl\": \"%.2f\", \
              \"amk_temp_igbt_fl\": \"%.2f\", \
              \"amk_error_info_fl\": \"%i\", \
              \
              \"amk_temp_motor_fr\": \"%.2f\", \
              \"amk_temp_inverter_fr\": \"%.2f\", \
              \"amk_temp_igbt_fr\": \"%.2f\", \
              \"amk_error_info_fr\": \"%i\", \
              \
              \"amk_temp_motor_rr\": \"%.2f\", \
              \"amk_temp_inverter_rr\": \"%.2f\", \
              \"amk_temp_igbt_rr\": \"%.2f\", \
              \"amk_error_info_rr\": \"%i\", \
              \
              \"amk_temp_motor_rl\": \"%.2f\", \
              \"amk_temp_inverter_rl\": \"%.2f\", \
              \"amk_temp_igbt_rl\": \"%.2f\", \
              \"amk_error_info_rl\": \"%i\", \
              \
              \"amk_torque_limit_positive_fl\": \"%i\", \
              \"amk_torque_limit_negative_fl\": \"%i\", \
              \"amk_torque_limit_positive_fr\": \"%i\", \
              \"amk_torque_limit_negative_fr\": \"%i\", \
              \"amk_torque_limit_positive_rr\": \"%i\", \
              \"amk_torque_limit_negative_rr\": \"%i\", \
              \"amk_torque_limit_positive_rl\": \"%i\", \
              \"amk_torque_limit_negative_rl\": \"%i\", \
              \
              \"throttle\": \"%i\", \
              \"steering_angle\": \"%i\", \
              \"brake\": \"%i\", \
              \"brake_press\": \"%i\", \
              \"car_status\": \"%lu\", \
              \"actual_velocity_kmh\": \"%i\", \
              \"brk_req\": \"%i\", \
              \"thr_req\": \"%i\", \
              \
              \"max_hv_volt\": \"%.2f\", \
              \"min_hv_volt\": \"%.2f\", \
              \"avg_hv_volt\": \"%.2f\", \
              \"max_hv_temp\": \"%.2f\", \
              \"min_hv_temp\": \"%.2f\", \
              \"avg_hv_temp\": \"%.2f\", \
              \"bms_error_map\": \"%i\", \
              \
              \"car_voltage\": \"%.2f\", \
              \"lem_current\": \"%.2f\", \
              \"current_sens\": \"%.2f\", \
              \"total_power\": \"%.2f\", \
              \
              \"fan_speed_r\": \"%i\", \
              \"fan_speed_l\": \"%i\", \
              \
              \"acceleration_x\": \"%.2f\", \
              \"acceleration_y\": \"%.2f\", \
              \"acceleration_z\": \"%.2f\", \
              \"omega_x\": \"%.2f\", \
              \"omega_y\": \"%.2f\", \
              \"omega_z\": \"%.2f\", \
              \
              \"suspensions_rl\": \"%.2f\", \
              \"suspensions_fl\": \"%.2f\", \
              \"suspensions_fr\": \"%.2f\", \
              \"suspensions_rr\": \"%.2f\", \
              \
              \"temp_inv_dx_2\": \"%.2f\", \
              \"temp_inv_sx_1\": \"%.2f\", \
              \"temp_inv_dx_1\": \"%.2f\", \
              \"temp_inv_sx_2\": \"%.2f\", \
              \"temp_mot_rr_1\": \"%.2f\", \
              \"temp_mot_rl_1\": \"%.2f\", \
              \"temp_mot_rl_2\": \"%.2f\", \
              \"temp_mot_rr_2\": \"%.2f\", \
              \
              \"gpio_bms\": \"%i\", \
              \"gpio_imd\": \"%i\", \
              \"max_temp_n_slave\": \"%i\", \
              \
              \"acc_pot_1\": \"%.2f\", \
              \"acc_pot_2\": \"%.2f\", \
              \"brk_pot\": \"%.2f\" \
            }",
                               ecu2.bms_lv[0],
                               ecu2.bms_lv[1],
                               ecu2.bms_lv[2],
                               ecu2.bms_lv[3],
                               ecu2.bms_lv[4],
                               ecu2.bms_lv[5],
                               ecu2.bms_lv[6],
                               ecu2.bms_lv[7],

                               (
                                   ecu2.motorVal1[0].AMK_bSystemReady << 7 &
                                   ecu2.motorVal1[0].AMK_bError << 6 &
                                   ecu2.motorVal1[0].AMK_bWarn << 5 &
                                   ecu2.motorVal1[0].AMK_bQuitDcOn << 4 &
                                   ecu2.motorVal1[0].AMK_bDcOn << 3 &
                                   ecu2.motorVal1[0].AMK_bQuitInverterOn << 2 &
                                   ecu2.motorVal1[0].AMK_bInverterOn << 1 &
                                   ecu2.motorVal1[0].AMK_bDerating),
                               ecu2.motorVal1[0].AMK_ActualVelocity,
                               ecu2.motorVal1[0].AMK_TorqueCurrent,
                               ecu2.motorVal1[0].AMK_Voltage,
                               ecu2.motorVal1[0].AMK_Current,

                               (
                                   ecu2.motorVal1[1].AMK_bSystemReady << 7 &
                                   ecu2.motorVal1[1].AMK_bError << 6 &
                                   ecu2.motorVal1[1].AMK_bWarn << 5 &
                                   ecu2.motorVal1[1].AMK_bQuitDcOn << 4 &
                                   ecu2.motorVal1[1].AMK_bDcOn << 3 &
                                   ecu2.motorVal1[1].AMK_bQuitInverterOn << 2 &
                                   ecu2.motorVal1[1].AMK_bInverterOn << 1 &
                                   ecu2.motorVal1[1].AMK_bDerating),
                               ecu2.motorVal1[1].AMK_ActualVelocity,
                               ecu2.motorVal1[1].AMK_TorqueCurrent,
                               ecu2.motorVal1[1].AMK_Voltage,
                               ecu2.motorVal1[1].AMK_Current,
                               (
                                   ecu2.motorVal1[2].AMK_bSystemReady << 7 &
                                   ecu2.motorVal1[2].AMK_bError << 6 &
                                   ecu2.motorVal1[2].AMK_bWarn << 5 &
                                   ecu2.motorVal1[2].AMK_bQuitDcOn << 4 &
                                   ecu2.motorVal1[2].AMK_bDcOn << 3 &
                                   ecu2.motorVal1[2].AMK_bQuitInverterOn << 2 &
                                   ecu2.motorVal1[2].AMK_bInverterOn << 1 &
                                   ecu2.motorVal1[2].AMK_bDerating),
                               ecu2.motorVal1[2].AMK_ActualVelocity,
                               ecu2.motorVal1[2].AMK_TorqueCurrent,
                               ecu2.motorVal1[2].AMK_Voltage,
                               ecu2.motorVal1[2].AMK_Current,
                               (
                                   ecu2.motorVal1[3].AMK_bSystemReady << 7 &
                                   ecu2.motorVal1[3].AMK_bError << 6 &
                                   ecu2.motorVal1[3].AMK_bWarn << 5 &
                                   ecu2.motorVal1[3].AMK_bQuitDcOn << 4 &
                                   ecu2.motorVal1[3].AMK_bDcOn << 3 &
                                   ecu2.motorVal1[3].AMK_bQuitInverterOn << 2 &
                                   ecu2.motorVal1[3].AMK_bInverterOn << 1 &
                                   ecu2.motorVal1[3].AMK_bDerating),
                               ecu2.motorVal1[3].AMK_ActualVelocity,
                               ecu2.motorVal1[3].AMK_TorqueCurrent,
                               ecu2.motorVal1[3].AMK_Voltage,
                               ecu2.motorVal1[3].AMK_Current,

                               ecu2.motorVal2[0].AMK_TempMotor,
                               ecu2.motorVal2[0].AMK_TempInverter,
                               ecu2.motorVal2[0].AMK_TempIGBT,
                               ecu2.motorVal2[0].AMK_ErrorInfo,

                               ecu2.motorVal2[1].AMK_TempMotor,
                               ecu2.motorVal2[1].AMK_TempInverter,
                               ecu2.motorVal2[1].AMK_TempIGBT,
                               ecu2.motorVal2[1].AMK_ErrorInfo,

                               ecu2.motorVal2[2].AMK_TempMotor,
                               ecu2.motorVal2[2].AMK_TempInverter,
                               ecu2.motorVal2[2].AMK_TempIGBT,
                               ecu2.motorVal2[2].AMK_ErrorInfo,

                               ecu2.motorVal2[3].AMK_TempMotor,
                               ecu2.motorVal2[3].AMK_TempInverter,
                               ecu2.motorVal2[3].AMK_TempIGBT,
                               ecu2.motorVal2[3].AMK_ErrorInfo,

                               ecu2.motorSetP[0].AMK_TorqueLimitPositive,
                               ecu2.motorSetP[0].AMK_TorqueLimitNegative,
                               ecu2.motorSetP[1].AMK_TorqueLimitPositive,
                               ecu2.motorSetP[1].AMK_TorqueLimitNegative,
                               ecu2.motorSetP[2].AMK_TorqueLimitPositive,
                               ecu2.motorSetP[2].AMK_TorqueLimitNegative,
                               ecu2.motorSetP[3].AMK_TorqueLimitPositive,
                               ecu2.motorSetP[3].AMK_TorqueLimitNegative,

                               ecu2.status.throttle_shared,
                               ecu2.status.steering_shared,
                               ecu2.status.brake_shared,
                               ecu2.status.brakePress_shared,
                               ecu2.status.status_shared,
                               ecu2.status.actualVelocityKMH_shared,
                               ecu2.pedals.brk_req_shared,
                               ecu2.pedals.throttle_req_shared,

                               ecu2.bms.max_bms_voltage_shared,
                               ecu2.bms.min_bms_voltage_shared,
                               ecu2.bms.mean_bms_voltage_shared,
                               ecu2.bms.max_bms_temp_shared,
                               ecu2.bms.min_bms_temp_shared,
                               ecu2.bms.mean_bms_temp_shared,
                               ecu2.bms.bms_bitmap_shared,

                               ecu2.power.batteryPack_voltage_shared,
                               ecu2.power.lem_current_shared,
                               ecu2.power.curr_sens_shared,
                               ecu2.power.total_power_shared,

                               ecu2.fanSpeed.rightFanSpeed_shared,
                               ecu2.fanSpeed.leftFanSpeed_shared,

                               ecu2.imu.accelerations_shared[0],
                               ecu2.imu.accelerations_shared[1],
                               ecu2.imu.accelerations_shared[2],
                               ecu2.imu.omegas_shared[0],
                               ecu2.imu.omegas_shared[1],
                               ecu2.imu.omegas_shared[2],

                               ecu2.imu.suspensions_shared[0],
                               ecu2.imu.suspensions_shared[1],
                               ecu2.imu.suspensions_shared[2],
                               ecu2.imu.suspensions_shared[3],

                               ecu2.imu.temperatures_shared[0],
                               ecu2.imu.temperatures_shared[1],
                               ecu2.imu.temperatures_shared[2],
                               ecu2.imu.temperatures_shared[3],
                               ecu2.imu.temperatures_shared[4],
                               ecu2.imu.temperatures_shared[5],
                               ecu2.imu.temperatures_shared[6],
                               ecu2.imu.temperatures_shared[7],

                               ecu2.gpio.Bms_shared,
                               ecu2.gpio.Imd_shared,
                               ecu2.bms.max_bms_temp_nslave_shared,

                               ecu2.pedals.acc_pot1_shared,
                               ecu2.pedals.acc_pot2_shared,
                               ecu2.pedals.brk_pot_shared);

        esp_http_client_set_post_field(http_client, body, body_len);
        esp_http_client_perform(http_client);

        ESP_LOGI("DONE", "SUPABASE DONE");

        memset(body, 0, 24500);
        xSemaphoreGive(can_insert);
      }
      else
        ESP_LOGI("S", "Semaphore not taken");
    }
    else
      ESP_LOGI("Q", "Nothing in queue");
  }
}

void serial_receive()
{
  char *data = (char *)malloc(BUF_SIZE);

  uart_event_t uart_event;
  while (1)
  {
    if (xQueueReceive(uart_queue, (void *)&uart_event, portTICK_PERIOD_MS))
    {
      switch (uart_event.type)
      {
      case UART_PATTERN_DET:
      {
        int pattern_pos = uart_pattern_pop_pos(UART_NUM);
        int read_len = uart_read_bytes(UART_NUM, data, pattern_pos, pdMS_TO_TICKS(100));
        uart_flush_input(UART_NUM);

        if (read_len - 1 == sizeof(struct logs))
        {
          uint8_t decoded[read_len - 1];
          cobs_decode(decoded, sizeof(decoded), data, read_len);
          memcpy(&ecu, &decoded, sizeof(decoded));

          xQueueSend(supabase_q, &ecu, 0);

          memset(data, 0, BUF_SIZE);
        }
      }
      default:
        break;
      }
    }
  }
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