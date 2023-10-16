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
bool connected = true;
#define WIFI_MAXIMUM_RETRY 10
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1
static int s_retry_num = 0;
static EventGroupHandle_t s_wifi_event_group;

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

// SUPABASE CONFIGURATION
#include "esp_http_client.h"
#define HTTP_SERVER_URL "https://wvjiwtbxvehgbdskepdn.supabase.co/rest/v1/test"
#define API_KEY "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Ind2aml3dGJ4dmVoZ2Jkc2tlcGRuIiwicm9sZSI6ImFub24iLCJpYXQiOjE2OTM4MjEzNzMsImV4cCI6MjAwOTM5NzM3M30.-QJcyNwn_4Dtkvohg-g3GJPb0jCoePxB1MEH7tzFbbo"
#define BEARER_VALUE "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Ind2aml3dGJ4dmVoZ2Jkc2tlcGRuIiwicm9sZSI6ImFub24iLCJpYXQiOjE2OTM4MjEzNzMsImV4cCI6MjAwOTM5NzM3M30.-QJcyNwn_4Dtkvohg-g3GJPb0jCoePxB1MEH7tzFbbo"
extern const char ssl_cert_pem_start[] asm("_binary_ssl_cert_pem_start");
extern const char ssl_cert_pem_end[] asm("_binary_ssl_cert_pem_end");

// COBS CONFIGURATION
#include "cobs.h"

// LOGS CONFIGURATION
struct motorValues1
{
  uint16_t AMK_bSystemReady;      // bool  
  uint16_t AMK_bError;            // bool
  uint16_t AMK_bWarn;             // bool
  uint16_t AMK_bQuitDcOn;         // bool
  uint16_t AMK_bDcOn;             // bool
  uint16_t AMK_bQuitInverterOn;   // bool
  uint16_t AMK_bInverterOn;       // bool
  uint16_t AMK_bDerating;         // bool

  float AMK_ActualVelocity;
  float AMK_TorqueCurrent;
  uint16_t AMK_Voltage;
  float AMK_Current;
};

struct motorValues2
{
  float AMK_TempMotor;
  float AMK_TempInverter;
  float AMK_TempIGBT;
  uint16_t AMK_ErrorInfo;     // unsigned int
};

struct motorSetPoints
{
  uint16_t AMK_bInverterOn;       // bool
  uint16_t AMK_bDcOn;             // bool
  uint16_t AMK_bEnable;           // bool
  uint16_t AMK_bErrorReset;       // bool

  int16_t AMK_TargetVelocity;        // int
  int16_t AMK_TorqueLimitPositive;   // int
  int16_t AMK_TorqueLimitNegative;   // int
};

struct Imu_Log
{
  float accelerations_shared[3];
  float omegas_shared[3];
  float temperatures_shared[8];
  float suspensions_shared[4];
};

struct FanSpeed_Log
{
  uint16_t leftFanSpeed_shared;
  uint16_t rightFanSpeed_shared;
};

struct Power_Log
{
  float lem_current_shared;
  float batteryPack_voltage_shared;
  float curr_sens_shared;
  float total_power_shared;
};

struct BMS_Log
{
  float max_bms_voltage_shared;
  float min_bms_voltage_shared;
  float mean_bms_voltage_shared;
  float max_bms_temp_shared;
  float min_bms_temp_shared;
  float mean_bms_temp_shared;
  uint16_t max_bms_temp_nslave_shared;
  uint16_t bms_bitmap_shared;
};

struct Status_Log
{
  int16_t throttle_shared;        // int
  int16_t steering_shared;        // int
  int16_t brake_shared;           // int
  int16_t brakePress_shared;      // int
  uint32_t status_shared;
  int16_t actualVelocityKMH_shared;    // int
};

struct Gpio_Log
{
  uint16_t Imd_shared;
  uint16_t Bms_shared;
  uint16_t Sdc1_shared;
  uint16_t Sdc2_shared;
  uint16_t Sdc3_shared;
  uint16_t Sdc4_shared;
  uint16_t Sdc5_shared;
  uint16_t Sdc6_shared;
};

struct Pedals_Log
{
  float acc_pot1_shared;
  float acc_pot2_shared;
  float brk_pot_shared;
  int16_t brk_req_shared;       // int
  int16_t throttle_req_shared;  // int

  uint16_t acc1_low_calibration;
  uint16_t acc2_low_calibration;
  uint16_t acc1_high_calibration;
  uint16_t acc2_high_calibration;
  uint16_t brk_low_calibration;
  uint16_t brk_high_calibration;
};

struct Power_Setup_Log
{
  int16_t max_speed_shared;         // int
  float rear_motor_scale_shared;
  float front_motor_scale_shared;
  float power_limit_shared;
};

struct logs
{
  float bms_lv[8];
  struct motorValues1 motorVal1[4];
  struct motorValues2 motorVal2[4];
  struct motorSetPoints motorSetP[4];
  struct Imu_Log imu;
  struct FanSpeed_Log fanSpeed;
  struct Power_Log power;
  struct BMS_Log bms;
  struct Status_Log status;
  struct Gpio_Log gpio;
  struct Pedals_Log pedals;
  struct Power_Setup_Log power_setup;
} *ecu;

SemaphoreHandle_t mySemaphore;

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

void db_insert(void *pvParameters)
{
  if (!connected)
    return;

  xSemaphoreTake(mySemaphore, portMAX_DELAY);

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
  char *body = malloc(24500);

  ESP_LOGI("Diocan1", "%i", ecu->gpio.Imd_shared);

  int body_len = sprintf(body, "{\
    \"bms_lv0\": \"%f\", \
    \"bms_lv1\": \"%f\", \
    \"bms_lv2\": \"%f\", \
    \"bms_lv3\": \"%f\", \
    \"bms_lv4\": \"%f\", \
    \"bms_lv5\": \"%f\", \
    \"bms_lv6\": \"%f\", \
    \"bms_lv7\": \"%f\", \
    \
    \"amk_status_fl\": \"%i\", \
    \"amk_actual_velocity_fl\": \"%f\", \
    \"amk_torque_current_fl\": \"%f\", \
    \"amk_voltage_fl\": \"%i\", \
    \"amk_current_fl\": \"%f\", \
    \
    \"amk_status_fr\": \"%i\", \
    \"amk_actual_velocity_fr\": \"%f\", \
    \"amk_torque_current_fr\": \"%f\", \
    \"amk_voltage_fr\": \"%i\", \
    \"amk_current_fr\": \"%f\", \
    \
    \"amk_status_rr\": \"%i\", \
    \"amk_actual_velocity_rr\": \"%f\", \
    \"amk_torque_current_rr\": \"%f\", \
    \"amk_voltage_rr\": \"%i\", \
    \"amk_current_rr\": \"%f\", \
    \
    \"amk_status_rl\": \"%i\", \
    \"amk_actual_velocity_rl\": \"%f\", \
    \"amk_torque_current_rl\": \"%f\", \
    \"amk_voltage_rl\": \"%i\", \
    \"amk_current_rl\": \"%f\", \
    \
    \"amk_temp_motor_fl\": \"%f\", \
    \"amk_temp_inverter_fl\": \"%f\", \
    \"amk_temp_igbt_fl\": \"%f\", \
    \"amk_error_info_fl\": \"%i\", \
    \
    \"amk_temp_motor_fr\": \"%f\", \
    \"amk_temp_inverter_fr\": \"%f\", \
    \"amk_temp_igbt_fr\": \"%f\", \
    \"amk_error_info_fr\": \"%i\", \
    \
    \"amk_temp_motor_rr\": \"%f\", \
    \"amk_temp_inverter_rr\": \"%f\", \
    \"amk_temp_igbt_rr\": \"%f\", \
    \"amk_error_info_rr\": \"%i\", \
    \
    \"amk_temp_motor_rl\": \"%f\", \
    \"amk_temp_inverter_rl\": \"%f\", \
    \"amk_temp_igbt_rl\": \"%f\", \
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
    \"max_hv_volt\": \"%f\", \
    \"min_hv_volt\": \"%f\", \
    \"avg_hv_volt\": \"%f\", \
    \"max_hv_temp\": \"%f\", \
    \"min_hv_temp\": \"%f\", \
    \"avg_hv_temp\": \"%f\", \
    \"bms_error_map\": \"%i\", \
    \
    \"car_voltage\": \"%f\", \
    \"lem_current\": \"%f\", \
    \"current_sens\": \"%f\", \
    \"total_power\": \"%f\", \
    \
    \"fan_speed_r\": \"%i\", \
    \"fan_speed_l\": \"%i\", \
    \
    \"acceleration_x\": \"%f\", \
    \"acceleration_y\": \"%f\", \
    \"acceleration_z\": \"%f\", \
    \"omega_x\": \"%f\", \
    \"omega_y\": \"%f\", \
    \"omega_z\": \"%f\", \
    \
    \"suspensions_rl\": \"%f\", \
    \"suspensions_fl\": \"%f\", \
    \"suspensions_fr\": \"%f\", \
    \"suspensions_rr\": \"%f\", \
    \
    \"temp_inv_dx_2\": \"%f\", \
    \"temp_inv_sx_1\": \"%f\", \
    \"temp_inv_dx_1\": \"%f\", \
    \"temp_inv_sx_2\": \"%f\", \
    \"temp_mot_rr_1\": \"%f\", \
    \"temp_mot_rl_1\": \"%f\", \
    \"temp_mot_rl_2\": \"%f\", \
    \"temp_mot_rr_2\": \"%f\", \
    \
    \"gpio_bms\": \"%i\", \
    \"gpio_imd\": \"%i\", \
    \"max_temp_n_slave\": \"%i\", \
    \
    \"acc_pot_1\": \"%f\", \
    \"acc_pot_2\": \"%f\", \
    \"brk_pot\": \"%f\" \
  }",
    ecu->bms_lv[0],
    ecu->bms_lv[1],
    ecu->bms_lv[2],
    ecu->bms_lv[3],
    ecu->bms_lv[4],
    ecu->bms_lv[5],
    ecu->bms_lv[6],
    ecu->bms_lv[7],

    (
      ecu->motorVal1[0].AMK_bSystemReady << 7 & 
      ecu->motorVal1[0].AMK_bError << 6 & 
      ecu->motorVal1[0].AMK_bWarn << 5 & 
      ecu->motorVal1[0].AMK_bQuitDcOn << 4 & 
      ecu->motorVal1[0].AMK_bDcOn << 3 & 
      ecu->motorVal1[0].AMK_bQuitInverterOn << 2 & 
      ecu->motorVal1[0].AMK_bInverterOn << 1 & 
      ecu->motorVal1[0].AMK_bDerating
    ),
    ecu->motorVal1[0].AMK_ActualVelocity,
    ecu->motorVal1[0].AMK_TorqueCurrent,
    ecu->motorVal1[0].AMK_Voltage,
    ecu->motorVal1[0].AMK_Current,

    (
      ecu->motorVal1[1].AMK_bSystemReady << 7 & 
      ecu->motorVal1[1].AMK_bError << 6 & 
      ecu->motorVal1[1].AMK_bWarn << 5 & 
      ecu->motorVal1[1].AMK_bQuitDcOn << 4 & 
      ecu->motorVal1[1].AMK_bDcOn << 3 & 
      ecu->motorVal1[1].AMK_bQuitInverterOn << 2 & 
      ecu->motorVal1[1].AMK_bInverterOn << 1 & 
      ecu->motorVal1[1].AMK_bDerating
    ),
    ecu->motorVal1[1].AMK_ActualVelocity,
    ecu->motorVal1[1].AMK_TorqueCurrent,
    ecu->motorVal1[1].AMK_Voltage,
    ecu->motorVal1[1].AMK_Current,
    (
      ecu->motorVal1[2].AMK_bSystemReady << 7 & 
      ecu->motorVal1[2].AMK_bError << 6 & 
      ecu->motorVal1[2].AMK_bWarn << 5 & 
      ecu->motorVal1[2].AMK_bQuitDcOn << 4 & 
      ecu->motorVal1[2].AMK_bDcOn << 3 & 
      ecu->motorVal1[2].AMK_bQuitInverterOn << 2 & 
      ecu->motorVal1[2].AMK_bInverterOn << 1 & 
      ecu->motorVal1[2].AMK_bDerating
    ),
    ecu->motorVal1[2].AMK_ActualVelocity,
    ecu->motorVal1[2].AMK_TorqueCurrent,
    ecu->motorVal1[2].AMK_Voltage,
    ecu->motorVal1[2].AMK_Current,
    (
      ecu->motorVal1[3].AMK_bSystemReady << 7 & 
      ecu->motorVal1[3].AMK_bError << 6 & 
      ecu->motorVal1[3].AMK_bWarn << 5 & 
      ecu->motorVal1[3].AMK_bQuitDcOn << 4 & 
      ecu->motorVal1[3].AMK_bDcOn << 3 & 
      ecu->motorVal1[3].AMK_bQuitInverterOn << 2 & 
      ecu->motorVal1[3].AMK_bInverterOn << 1 & 
      ecu->motorVal1[3].AMK_bDerating
    ),
    ecu->motorVal1[3].AMK_ActualVelocity,
    ecu->motorVal1[3].AMK_TorqueCurrent,
    ecu->motorVal1[3].AMK_Voltage,
    ecu->motorVal1[3].AMK_Current,

    ecu->motorVal2[0].AMK_TempMotor,
    ecu->motorVal2[0].AMK_TempInverter,
    ecu->motorVal2[0].AMK_TempIGBT,
    ecu->motorVal2[0].AMK_ErrorInfo,

    ecu->motorVal2[1].AMK_TempMotor,
    ecu->motorVal2[1].AMK_TempInverter,
    ecu->motorVal2[1].AMK_TempIGBT,
    ecu->motorVal2[1].AMK_ErrorInfo,

    ecu->motorVal2[2].AMK_TempMotor,
    ecu->motorVal2[2].AMK_TempInverter,
    ecu->motorVal2[2].AMK_TempIGBT,
    ecu->motorVal2[2].AMK_ErrorInfo,

    ecu->motorVal2[3].AMK_TempMotor,
    ecu->motorVal2[3].AMK_TempInverter,
    ecu->motorVal2[3].AMK_TempIGBT,
    ecu->motorVal2[3].AMK_ErrorInfo,

    ecu->motorSetP[0].AMK_TorqueLimitPositive,
    ecu->motorSetP[0].AMK_TorqueLimitNegative,
    ecu->motorSetP[1].AMK_TorqueLimitPositive,
    ecu->motorSetP[1].AMK_TorqueLimitNegative,
    ecu->motorSetP[2].AMK_TorqueLimitPositive,
    ecu->motorSetP[2].AMK_TorqueLimitNegative,
    ecu->motorSetP[3].AMK_TorqueLimitPositive,
    ecu->motorSetP[3].AMK_TorqueLimitNegative,

    ecu->status.throttle_shared,
    ecu->status.steering_shared,
    ecu->status.brake_shared,
    ecu->status.brakePress_shared,
    ecu->status.status_shared,
    ecu->status.actualVelocityKMH_shared,
    ecu->pedals.brk_req_shared,
    ecu->pedals.throttle_req_shared,

    ecu->bms.max_bms_voltage_shared,
    ecu->bms.min_bms_voltage_shared,
    ecu->bms.mean_bms_voltage_shared,
    ecu->bms.max_bms_temp_shared,
    ecu->bms.min_bms_temp_shared,
    ecu->bms.mean_bms_temp_shared,
    ecu->bms.bms_bitmap_shared,

    ecu->power.batteryPack_voltage_shared,
    ecu->power.lem_current_shared,
    ecu->power.curr_sens_shared,
    ecu->power.total_power_shared,

    ecu->fanSpeed.rightFanSpeed_shared,
    ecu->fanSpeed.leftFanSpeed_shared,

    ecu->imu.accelerations_shared[0],
    ecu->imu.accelerations_shared[1],
    ecu->imu.accelerations_shared[2],
    ecu->imu.omegas_shared[0],
    ecu->imu.omegas_shared[1],
    ecu->imu.omegas_shared[2],

    ecu->imu.suspensions_shared[0],
    ecu->imu.suspensions_shared[1],
    ecu->imu.suspensions_shared[2],
    ecu->imu.suspensions_shared[3],

    ecu->imu.temperatures_shared[0],
    ecu->imu.temperatures_shared[1],
    ecu->imu.temperatures_shared[2],
    ecu->imu.temperatures_shared[3],
    ecu->imu.temperatures_shared[4],
    ecu->imu.temperatures_shared[5],
    ecu->imu.temperatures_shared[6],
    ecu->imu.temperatures_shared[7],

    ecu->gpio.Bms_shared,
    ecu->gpio.Imd_shared,
    ecu->bms.max_bms_temp_nslave_shared,

    ecu->pedals.acc_pot1_shared,
    ecu->pedals.acc_pot2_shared,
    ecu->pedals.brk_pot_shared
  );

  ESP_LOGI("Diocan2", "%i", ecu->gpio.Imd_shared);

  esp_http_client_set_post_field(http_client, body, body_len);
  esp_http_client_perform(http_client);
  esp_http_client_cleanup(http_client);

  vTaskDelete(NULL);
}

void serial_receive()
{
  char* data = (char *)malloc(BUF_SIZE);

  uart_event_t uart_event;
  while (1)
  {
    if (xQueueReceive(uart_queue, (void *)&uart_event, pdMS_TO_TICKS(100)))
    {
      switch (uart_event.type)
      {
      case UART_PATTERN_DET:
      //case UART_DATA:
        {
          int pattern_pos = uart_pattern_pop_pos(UART_NUM);
          int read_len = uart_read_bytes(UART_NUM, data, pattern_pos, pdMS_TO_TICKS(100));
          uart_flush_input(UART_NUM);

          if(read_len - 1 == sizeof(struct logs)) {
            uint8_t decoded[read_len-1];
            cobs_decode(decoded, sizeof(decoded), data, read_len);
            ecu = (struct logs*) decoded;
            xSemaphoreGive(mySemaphore);  // Segnala il completamento

            ESP_LOGI("Diocan", "%i", ecu->gpio.Imd_shared);

            xTaskCreate(db_insert, "db_insert", 24576, NULL, tskIDLE_PRIORITY, NULL);
          }

          memset(data, 0, BUF_SIZE);
          uart_flush_input(UART_NUM);
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

  mySemaphore = xSemaphoreCreateBinary();
  
  wifi_init_sta();
  serial_init();

  serial_receive();
}