#include <stdio.h>
#include <time.h>
#include "databasec.h"

void database_insert()
{
    int i = 0; // JSON (select) counter
    char body[BODY_MAX_SIZE];
    struct logs ecu;

    // continuous task
    while (1)
    {
        bool uart_cond = xQueueReceive(ecu_data, &ecu, pdMS_TO_TICKS(1000)) == pdPASS;
        // check if connected to wifi, struct arrived and sempahore taken
        if (connected && uart_cond)
        {
            struct timeval tv_now;
            gettimeofday(&tv_now, NULL);
            int64_t time_ms = (int64_t)tv_now.tv_sec * 1000L + (int64_t)tv_now.tv_usec / 1000;
            ESP_LOGI("DB", "Unix timestamp (ms): %lli", time_ms);
            int body_len = 0;
            // Most important data
            #ifndef SPLITTED
                body_len = sprintf(
                    body,
                    JSON,
                    stest,
                    time_ms,
                    ecu.bms_lv[0], // bms lv voltages and temperatures
                    ecu.bms_lv[1],
                    ecu.bms_lv[2],
                    ecu.bms_lv[3],
                    ecu.bms_lv[4],
                    ecu.bms_lv[5],
                    ecu.bms_lv[6],
                    ecu.bms_lv[7],
                    // FL motor status and actual velocity
                    (ecu.motorVal1[0].AMK_bSystemReady << 7 &
                     ecu.motorVal1[0].AMK_bError << 6 &
                     ecu.motorVal1[0].AMK_bWarn << 5 &
                     ecu.motorVal1[0].AMK_bQuitDcOn << 4 &
                     ecu.motorVal1[0].AMK_bDcOn << 3 &
                     ecu.motorVal1[0].AMK_bQuitInverterOn << 2 &
                     ecu.motorVal1[0].AMK_bInverterOn << 1 &
                     ecu.motorVal1[0].AMK_bDerating),
                    ecu.motorVal1[0].AMK_ActualVelocity,
                    // FR motor status and actual velocity
                    (ecu.motorVal1[1].AMK_bSystemReady << 7 &
                     ecu.motorVal1[1].AMK_bError << 6 &
                     ecu.motorVal1[1].AMK_bWarn << 5 &
                     ecu.motorVal1[1].AMK_bQuitDcOn << 4 &
                     ecu.motorVal1[1].AMK_bDcOn << 3 &
                     ecu.motorVal1[1].AMK_bQuitInverterOn << 2 &
                     ecu.motorVal1[1].AMK_bInverterOn << 1 &
                     ecu.motorVal1[1].AMK_bDerating),
                    ecu.motorVal1[1].AMK_ActualVelocity,
                    // RR motor status and actual velocity
                    (ecu.motorVal1[2].AMK_bSystemReady << 7 &
                     ecu.motorVal1[2].AMK_bError << 6 &
                     ecu.motorVal1[2].AMK_bWarn << 5 &
                     ecu.motorVal1[2].AMK_bQuitDcOn << 4 &
                     ecu.motorVal1[2].AMK_bDcOn << 3 &
                     ecu.motorVal1[2].AMK_bQuitInverterOn << 2 &
                     ecu.motorVal1[2].AMK_bInverterOn << 1 &
                     ecu.motorVal1[2].AMK_bDerating),
                    ecu.motorVal1[2].AMK_ActualVelocity,
                    // RL motor status and actual velocity
                    (ecu.motorVal1[3].AMK_bSystemReady << 7 &
                     ecu.motorVal1[3].AMK_bError << 6 &
                     ecu.motorVal1[3].AMK_bWarn << 5 &
                     ecu.motorVal1[3].AMK_bQuitDcOn << 4 &
                     ecu.motorVal1[3].AMK_bDcOn << 3 &
                     ecu.motorVal1[3].AMK_bQuitInverterOn << 2 &
                     ecu.motorVal1[3].AMK_bInverterOn << 1 &
                     ecu.motorVal1[3].AMK_bDerating),
                    ecu.motorVal1[3].AMK_ActualVelocity,
                    // Torque limits
                    ecu.motorSetP[0].AMK_TorqueLimitPositive,
                    ecu.motorSetP[0].AMK_TorqueLimitNegative,
                    ecu.motorSetP[1].AMK_TorqueLimitPositive,
                    ecu.motorSetP[1].AMK_TorqueLimitNegative,
                    ecu.motorSetP[2].AMK_TorqueLimitPositive,
                    ecu.motorSetP[2].AMK_TorqueLimitNegative,
                    ecu.motorSetP[3].AMK_TorqueLimitPositive,
                    ecu.motorSetP[3].AMK_TorqueLimitNegative,
                    // Driver inputs
                    ecu.status.throttle_shared,
                    ecu.status.steering_shared,
                    ecu.status.brake_shared,
                    ecu.status.actualVelocityKMH_shared,
                    // Most important bms hv data
                    ecu.bms.max_bms_voltage_shared,
                    ecu.bms.min_bms_voltage_shared,
                    ecu.bms.mean_bms_voltage_shared,
                    ecu.bms.max_bms_temp_shared,
                    ecu.bms.min_bms_temp_shared,
                    ecu.bms.mean_bms_temp_shared,
                    // Power info
                    ecu.power.batteryPack_voltage_shared,
                    ecu.power.lem_current_shared,
                    // IMU and SMU
                    ecu.imu.accelerations_shared[0],
                    ecu.imu.accelerations_shared[1],
                    ecu.imu.accelerations_shared[2],
                    ecu.imu.omegas_shared[0],
                    ecu.imu.omegas_shared[1],
                    ecu.imu.omegas_shared[2],
                    ecu.imu.suspensions_shared[0],
                    ecu.imu.suspensions_shared[1],
                    ecu.imu.suspensions_shared[2],
                    ecu.imu.suspensions_shared[3],
                    // FL motor
                    ecu.motorVal1[0].AMK_TorqueCurrent,
                    ecu.motorVal1[0].AMK_Voltage,
                    ecu.motorVal1[0].AMK_Current,
                    // FR motor
                    ecu.motorVal1[1].AMK_TorqueCurrent,
                    ecu.motorVal1[1].AMK_Voltage,
                    ecu.motorVal1[1].AMK_Current,
                    // RR motor
                    ecu.motorVal1[2].AMK_TorqueCurrent,
                    ecu.motorVal1[2].AMK_Voltage,
                    ecu.motorVal1[2].AMK_Current,
                    // RL motor
                    ecu.motorVal1[3].AMK_TorqueCurrent,
                    ecu.motorVal1[3].AMK_Voltage,
                    ecu.motorVal1[3].AMK_Current,
                    // FL motor values
                    ecu.motorVal2[0].AMK_TempMotor,
                    ecu.motorVal2[0].AMK_TempInverter,
                    ecu.motorVal2[0].AMK_TempIGBT,
                    ecu.motorVal2[0].AMK_ErrorInfo,
                    // FR motor values
                    ecu.motorVal2[1].AMK_TempMotor,
                    ecu.motorVal2[1].AMK_TempInverter,
                    ecu.motorVal2[1].AMK_TempIGBT,
                    ecu.motorVal2[1].AMK_ErrorInfo,
                    // RR motor values
                    ecu.motorVal2[2].AMK_TempMotor,
                    ecu.motorVal2[2].AMK_TempInverter,
                    ecu.motorVal2[2].AMK_TempIGBT,
                    ecu.motorVal2[2].AMK_ErrorInfo,
                    // RL motor values
                    ecu.motorVal2[3].AMK_TempMotor,
                    ecu.motorVal2[3].AMK_TempInverter,
                    ecu.motorVal2[3].AMK_TempIGBT,
                    ecu.motorVal2[3].AMK_ErrorInfo,
                    // Other driver inputs
                    ecu.status.brakePress_shared,
                    ecu.status.status_shared,
                    ecu.pedals.brk_req_shared,
                    ecu.pedals.throttle_req_shared,
                    // Bms hv error map
                    ecu.bms.bms_bitmap_shared,
                    // Other power values
                    ecu.power.curr_sens_shared,
                    ecu.power.total_power_shared,
                    // Fan data
                    ecu.fanSpeed.rightFanSpeed_shared,
                    ecu.fanSpeed.leftFanSpeed_shared,
                    // Pre/post motor/inverter values - if someone (culi) asks, we don't know which one is what :)
                    ecu.imu.temperatures_shared[0],
                    ecu.imu.temperatures_shared[1],
                    ecu.imu.temperatures_shared[2],
                    ecu.imu.temperatures_shared[3],
                    ecu.imu.temperatures_shared[4],
                    ecu.imu.temperatures_shared[5],
                    ecu.imu.temperatures_shared[6],
                    ecu.imu.temperatures_shared[7],
                    // Errors
                    ecu.gpio.Bms_shared,
                    ecu.gpio.Imd_shared,
                    ecu.bms.max_bms_temp_nslave_shared,
                    // Potentiometers
                    ecu.pedals.acc_pot1_shared,
                    ecu.pedals.acc_pot2_shared,
                    ecu.pedals.brk_pot_shared);
            #else
            if (i != 10)
            {
                body_len = sprintf(
                    body,
                    JSON_1,
                    stest,
                    ecu.bms_lv[0], // bms lv voltages and temperatures
                    ecu.bms_lv[1],
                    ecu.bms_lv[2],
                    ecu.bms_lv[3],
                    ecu.bms_lv[4],
                    ecu.bms_lv[5],
                    ecu.bms_lv[6],
                    ecu.bms_lv[7],
                    // FL motor status and actual velocity
                    (ecu.motorVal1[0].AMK_bSystemReady << 7 &
                     ecu.motorVal1[0].AMK_bError << 6 &
                     ecu.motorVal1[0].AMK_bWarn << 5 &
                     ecu.motorVal1[0].AMK_bQuitDcOn << 4 &
                     ecu.motorVal1[0].AMK_bDcOn << 3 &
                     ecu.motorVal1[0].AMK_bQuitInverterOn << 2 &
                     ecu.motorVal1[0].AMK_bInverterOn << 1 &
                     ecu.motorVal1[0].AMK_bDerating),
                    ecu.motorVal1[0].AMK_ActualVelocity,
                    // FR motor status and actual velocity
                    (ecu.motorVal1[1].AMK_bSystemReady << 7 &
                     ecu.motorVal1[1].AMK_bError << 6 &
                     ecu.motorVal1[1].AMK_bWarn << 5 &
                     ecu.motorVal1[1].AMK_bQuitDcOn << 4 &
                     ecu.motorVal1[1].AMK_bDcOn << 3 &
                     ecu.motorVal1[1].AMK_bQuitInverterOn << 2 &
                     ecu.motorVal1[1].AMK_bInverterOn << 1 &
                     ecu.motorVal1[1].AMK_bDerating),
                    ecu.motorVal1[1].AMK_ActualVelocity,
                    // RR motor status and actual velocity
                    (ecu.motorVal1[2].AMK_bSystemReady << 7 &
                     ecu.motorVal1[2].AMK_bError << 6 &
                     ecu.motorVal1[2].AMK_bWarn << 5 &
                     ecu.motorVal1[2].AMK_bQuitDcOn << 4 &
                     ecu.motorVal1[2].AMK_bDcOn << 3 &
                     ecu.motorVal1[2].AMK_bQuitInverterOn << 2 &
                     ecu.motorVal1[2].AMK_bInverterOn << 1 &
                     ecu.motorVal1[2].AMK_bDerating),
                    ecu.motorVal1[2].AMK_ActualVelocity,
                    // RL motor status and actual velocity
                    (ecu.motorVal1[3].AMK_bSystemReady << 7 &
                     ecu.motorVal1[3].AMK_bError << 6 &
                     ecu.motorVal1[3].AMK_bWarn << 5 &
                     ecu.motorVal1[3].AMK_bQuitDcOn << 4 &
                     ecu.motorVal1[3].AMK_bDcOn << 3 &
                     ecu.motorVal1[3].AMK_bQuitInverterOn << 2 &
                     ecu.motorVal1[3].AMK_bInverterOn << 1 &
                     ecu.motorVal1[3].AMK_bDerating),
                    ecu.motorVal1[3].AMK_ActualVelocity,
                    // Torque limits
                    ecu.motorSetP[0].AMK_TorqueLimitPositive,
                    ecu.motorSetP[0].AMK_TorqueLimitNegative,
                    ecu.motorSetP[1].AMK_TorqueLimitPositive,
                    ecu.motorSetP[1].AMK_TorqueLimitNegative,
                    ecu.motorSetP[2].AMK_TorqueLimitPositive,
                    ecu.motorSetP[2].AMK_TorqueLimitNegative,
                    ecu.motorSetP[3].AMK_TorqueLimitPositive,
                    ecu.motorSetP[3].AMK_TorqueLimitNegative,
                    // Driver inputs
                    ecu.status.throttle_shared,
                    ecu.status.steering_shared,
                    ecu.status.brake_shared,
                    ecu.status.actualVelocityKMH_shared,
                    // Most important bms hv data
                    ecu.bms.max_bms_voltage_shared,
                    ecu.bms.min_bms_voltage_shared,
                    ecu.bms.mean_bms_voltage_shared,
                    ecu.bms.max_bms_temp_shared,
                    ecu.bms.min_bms_temp_shared,
                    ecu.bms.mean_bms_temp_shared,
                    // Power info
                    ecu.power.batteryPack_voltage_shared,
                    ecu.power.lem_current_shared,
                    // IMU and SMU
                    ecu.imu.accelerations_shared[0],
                    ecu.imu.accelerations_shared[1],
                    ecu.imu.accelerations_shared[2],
                    ecu.imu.omegas_shared[0],
                    ecu.imu.omegas_shared[1],
                    ecu.imu.omegas_shared[2],
                    ecu.imu.suspensions_shared[0],
                    ecu.imu.suspensions_shared[1],
                    ecu.imu.suspensions_shared[2],
                    ecu.imu.suspensions_shared[3]);
            }
            else
            { // Other data (less important ones)
                body_len = sprintf(
                    body,
                    JSON_2,
                    stest,
                    // FL motor
                    ecu.motorVal1[0].AMK_TorqueCurrent,
                    ecu.motorVal1[0].AMK_Voltage,
                    ecu.motorVal1[0].AMK_Current,
                    // FR motor
                    ecu.motorVal1[1].AMK_TorqueCurrent,
                    ecu.motorVal1[1].AMK_Voltage,
                    ecu.motorVal1[1].AMK_Current,
                    // RR motor
                    ecu.motorVal1[2].AMK_TorqueCurrent,
                    ecu.motorVal1[2].AMK_Voltage,
                    ecu.motorVal1[2].AMK_Current,
                    // RL motor
                    ecu.motorVal1[3].AMK_TorqueCurrent,
                    ecu.motorVal1[3].AMK_Voltage,
                    ecu.motorVal1[3].AMK_Current,
                    // FL motor values
                    ecu.motorVal2[0].AMK_TempMotor,
                    ecu.motorVal2[0].AMK_TempInverter,
                    ecu.motorVal2[0].AMK_TempIGBT,
                    ecu.motorVal2[0].AMK_ErrorInfo,
                    // FR motor values
                    ecu.motorVal2[1].AMK_TempMotor,
                    ecu.motorVal2[1].AMK_TempInverter,
                    ecu.motorVal2[1].AMK_TempIGBT,
                    ecu.motorVal2[1].AMK_ErrorInfo,
                    // RR motor values
                    ecu.motorVal2[2].AMK_TempMotor,
                    ecu.motorVal2[2].AMK_TempInverter,
                    ecu.motorVal2[2].AMK_TempIGBT,
                    ecu.motorVal2[2].AMK_ErrorInfo,
                    // RL motor values
                    ecu.motorVal2[3].AMK_TempMotor,
                    ecu.motorVal2[3].AMK_TempInverter,
                    ecu.motorVal2[3].AMK_TempIGBT,
                    ecu.motorVal2[3].AMK_ErrorInfo,
                    // Other driver inputs
                    ecu.status.brakePress_shared,
                    ecu.status.status_shared,
                    ecu.pedals.brk_req_shared,
                    ecu.pedals.throttle_req_shared,
                    // Bms hv error map
                    ecu.bms.bms_bitmap_shared,
                    // Other power values
                    ecu.power.curr_sens_shared,
                    ecu.power.total_power_shared,
                    // Fan data
                    ecu.fanSpeed.rightFanSpeed_shared,
                    ecu.fanSpeed.leftFanSpeed_shared,
                    // Pre/post motor/inverter values - if someone (culi) asks, we don't know which one is what :)
                    ecu.imu.temperatures_shared[0],
                    ecu.imu.temperatures_shared[1],
                    ecu.imu.temperatures_shared[2],
                    ecu.imu.temperatures_shared[3],
                    ecu.imu.temperatures_shared[4],
                    ecu.imu.temperatures_shared[5],
                    ecu.imu.temperatures_shared[6],
                    ecu.imu.temperatures_shared[7],
                    // Errors
                    ecu.gpio.Bms_shared,
                    ecu.gpio.Imd_shared,
                    ecu.bms.max_bms_temp_nslave_shared,
                    // Potentiometers
                    ecu.pedals.acc_pot1_shared,
                    ecu.pedals.acc_pot2_shared,
                    ecu.pedals.brk_pot_shared);

                // Reset counter
                i = -1;
            }
            #endif

            #if GZIP
            // Compress body
            z_stream strm;
            /* allocate deflate state */
            strm.zalloc = Z_NULL;
            strm.zfree = Z_NULL;
            strm.opaque = Z_NULL;
            if(deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 25, 7, Z_DEFAULT_STRATEGY) != Z_OK) {
                ESP_LOGE("GZIP", "Failed to init stream");
                deflateEnd(&strm);
                continue;
            }
            uLong compressed_size = deflateBound(&strm, body_len);
            Bytef* compressed_body = (Bytef*)malloc(compressed_size);
            strm.next_in = (Bytef *)body;
            strm.avail_in = body_len;
            strm.next_out = compressed_body;
            strm.avail_out = compressed_size;
            int ret = 0;
            if ((ret = deflate(&strm, Z_FINISH)) != Z_STREAM_END) {
                ESP_LOGE("GZIP", "Compression failed code %d", ret);
                deflateEnd(&strm);
                free(compressed_body);
                continue;
            }
            compressed_size = strm.total_out;
            deflateEnd(&strm); 
            ESP_LOGI("GZIP", "Compression done, original size %d, compressed size: %lu", body_len, compressed_size);

            esp_http_client_set_header(http_client, "Content-Encoding", "gzip");
            char http_request_len[10];
            esp_http_client_set_header(http_client, "Content-Length", itoa(compressed_size, http_request_len, 10));
            esp_http_client_set_post_field(http_client, (char*)compressed_body, compressed_size);
            esp_http_client_perform(http_client);
            ESP_LOGI("DB DONE", "Compressed JSON inserting done");
            free(compressed_body);
            #else 
            char http_request_len[10];
            esp_http_client_set_header(http_client, "Content-Length", itoa(body_len, http_request_len, 10));
            esp_http_client_set_post_field(http_client, body, body_len);
            esp_http_client_perform(http_client);
            ESP_LOGI("DB DONE", "JSON inserting done");
            #endif
            // Increment JSON counter
            i++;
            memset(body, 0, BODY_MAX_SIZE);
        }
        else
        {
            #ifndef DEBUG
            char* error_type = !connected ? "ESP not connected" : (!uart_cond ? "UART not received" : "Semaphore not taken");
            ESP_LOGE("DB", "%s", error_type);
            #endif
            gpio_set_level(DB_ERROR_GPIO, 1);
        }
    }
}
