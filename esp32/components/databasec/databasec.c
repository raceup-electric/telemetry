#include <stdio.h>
#include "databasec.h"

#define MINIZ_NO_ZLIB_APIS

void database_insert()
{
    int i = 0; // JSON (select) counter

    // char body[BODY_MAX_SIZE];
    char *body = (char *)malloc(BODY_MAX_SIZE);

    bool uart_cond = xQueueReceive(supabase_q, &ecu2, (TickType_t)10) == pdPASS;
    bool smph_cond = xSemaphoreTake(can_insert, 1) == pdTRUE;

    // continuous task
    while (1)
    {
        // check if connected to wifi, struct arrived and sempahore taken
        if (connected && uart_cond && smph_cond)
        {
            int body_len = 0;
            // Most important data
            #ifndef SPLITTED
                body_len = sprintf(
                    body,
                    JSON,
                    stest,
                    ecu2.bms_lv[0], // bms lv voltages and temperatures
                    ecu2.bms_lv[1],
                    ecu2.bms_lv[2],
                    ecu2.bms_lv[3],
                    ecu2.bms_lv[4],
                    ecu2.bms_lv[5],
                    ecu2.bms_lv[6],
                    ecu2.bms_lv[7],
                    // FL motor status and actual velocity
                    (ecu2.motorVal1[0].AMK_bSystemReady << 7 &
                     ecu2.motorVal1[0].AMK_bError << 6 &
                     ecu2.motorVal1[0].AMK_bWarn << 5 &
                     ecu2.motorVal1[0].AMK_bQuitDcOn << 4 &
                     ecu2.motorVal1[0].AMK_bDcOn << 3 &
                     ecu2.motorVal1[0].AMK_bQuitInverterOn << 2 &
                     ecu2.motorVal1[0].AMK_bInverterOn << 1 &
                     ecu2.motorVal1[0].AMK_bDerating),
                    ecu2.motorVal1[0].AMK_ActualVelocity,
                    // FR motor status and actual velocity
                    (ecu2.motorVal1[1].AMK_bSystemReady << 7 &
                     ecu2.motorVal1[1].AMK_bError << 6 &
                     ecu2.motorVal1[1].AMK_bWarn << 5 &
                     ecu2.motorVal1[1].AMK_bQuitDcOn << 4 &
                     ecu2.motorVal1[1].AMK_bDcOn << 3 &
                     ecu2.motorVal1[1].AMK_bQuitInverterOn << 2 &
                     ecu2.motorVal1[1].AMK_bInverterOn << 1 &
                     ecu2.motorVal1[1].AMK_bDerating),
                    ecu2.motorVal1[1].AMK_ActualVelocity,
                    // RR motor status and actual velocity
                    (ecu2.motorVal1[2].AMK_bSystemReady << 7 &
                     ecu2.motorVal1[2].AMK_bError << 6 &
                     ecu2.motorVal1[2].AMK_bWarn << 5 &
                     ecu2.motorVal1[2].AMK_bQuitDcOn << 4 &
                     ecu2.motorVal1[2].AMK_bDcOn << 3 &
                     ecu2.motorVal1[2].AMK_bQuitInverterOn << 2 &
                     ecu2.motorVal1[2].AMK_bInverterOn << 1 &
                     ecu2.motorVal1[2].AMK_bDerating),
                    ecu2.motorVal1[2].AMK_ActualVelocity,
                    // RL motor status and actual velocity
                    (ecu2.motorVal1[3].AMK_bSystemReady << 7 &
                     ecu2.motorVal1[3].AMK_bError << 6 &
                     ecu2.motorVal1[3].AMK_bWarn << 5 &
                     ecu2.motorVal1[3].AMK_bQuitDcOn << 4 &
                     ecu2.motorVal1[3].AMK_bDcOn << 3 &
                     ecu2.motorVal1[3].AMK_bQuitInverterOn << 2 &
                     ecu2.motorVal1[3].AMK_bInverterOn << 1 &
                     ecu2.motorVal1[3].AMK_bDerating),
                    ecu2.motorVal1[3].AMK_ActualVelocity,
                    // Torque limits
                    ecu2.motorSetP[0].AMK_TorqueLimitPositive,
                    ecu2.motorSetP[0].AMK_TorqueLimitNegative,
                    ecu2.motorSetP[1].AMK_TorqueLimitPositive,
                    ecu2.motorSetP[1].AMK_TorqueLimitNegative,
                    ecu2.motorSetP[2].AMK_TorqueLimitPositive,
                    ecu2.motorSetP[2].AMK_TorqueLimitNegative,
                    ecu2.motorSetP[3].AMK_TorqueLimitPositive,
                    ecu2.motorSetP[3].AMK_TorqueLimitNegative,
                    // Driver inputs
                    ecu2.status.throttle_shared,
                    ecu2.status.steering_shared,
                    ecu2.status.brake_shared,
                    ecu2.status.actualVelocityKMH_shared,
                    // Most important bms hv data
                    ecu2.bms.max_bms_voltage_shared,
                    ecu2.bms.min_bms_voltage_shared,
                    ecu2.bms.mean_bms_voltage_shared,
                    ecu2.bms.max_bms_temp_shared,
                    ecu2.bms.min_bms_temp_shared,
                    ecu2.bms.mean_bms_temp_shared,
                    // Power info
                    ecu2.power.batteryPack_voltage_shared,
                    ecu2.power.lem_current_shared,
                    // IMU and SMU
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
                    // FL motor
                    ecu2.motorVal1[0].AMK_TorqueCurrent,
                    ecu2.motorVal1[0].AMK_Voltage,
                    ecu2.motorVal1[0].AMK_Current,
                    // FR motor
                    ecu2.motorVal1[1].AMK_TorqueCurrent,
                    ecu2.motorVal1[1].AMK_Voltage,
                    ecu2.motorVal1[1].AMK_Current,
                    // RR motor
                    ecu2.motorVal1[2].AMK_TorqueCurrent,
                    ecu2.motorVal1[2].AMK_Voltage,
                    ecu2.motorVal1[2].AMK_Current,
                    // RL motor
                    ecu2.motorVal1[3].AMK_TorqueCurrent,
                    ecu2.motorVal1[3].AMK_Voltage,
                    ecu2.motorVal1[3].AMK_Current,
                    // FL motor values
                    ecu2.motorVal2[0].AMK_TempMotor,
                    ecu2.motorVal2[0].AMK_TempInverter,
                    ecu2.motorVal2[0].AMK_TempIGBT,
                    ecu2.motorVal2[0].AMK_ErrorInfo,
                    // FR motor values
                    ecu2.motorVal2[1].AMK_TempMotor,
                    ecu2.motorVal2[1].AMK_TempInverter,
                    ecu2.motorVal2[1].AMK_TempIGBT,
                    ecu2.motorVal2[1].AMK_ErrorInfo,
                    // RR motor values
                    ecu2.motorVal2[2].AMK_TempMotor,
                    ecu2.motorVal2[2].AMK_TempInverter,
                    ecu2.motorVal2[2].AMK_TempIGBT,
                    ecu2.motorVal2[2].AMK_ErrorInfo,
                    // RL motor values
                    ecu2.motorVal2[3].AMK_TempMotor,
                    ecu2.motorVal2[3].AMK_TempInverter,
                    ecu2.motorVal2[3].AMK_TempIGBT,
                    ecu2.motorVal2[3].AMK_ErrorInfo,
                    // Other driver inputs
                    ecu2.status.brakePress_shared,
                    ecu2.status.status_shared,
                    ecu2.pedals.brk_req_shared,
                    ecu2.pedals.throttle_req_shared,
                    // Bms hv error map
                    ecu2.bms.bms_bitmap_shared,
                    // Other power values
                    ecu2.power.curr_sens_shared,
                    ecu2.power.total_power_shared,
                    // Fan data
                    ecu2.fanSpeed.rightFanSpeed_shared,
                    ecu2.fanSpeed.leftFanSpeed_shared,
                    // Pre/post motor/inverter values - if someone (culi) asks, we don't know which one is what :)
                    ecu2.imu.temperatures_shared[0],
                    ecu2.imu.temperatures_shared[1],
                    ecu2.imu.temperatures_shared[2],
                    ecu2.imu.temperatures_shared[3],
                    ecu2.imu.temperatures_shared[4],
                    ecu2.imu.temperatures_shared[5],
                    ecu2.imu.temperatures_shared[6],
                    ecu2.imu.temperatures_shared[7],
                    // Errors
                    ecu2.gpio.Bms_shared,
                    ecu2.gpio.Imd_shared,
                    ecu2.bms.max_bms_temp_nslave_shared,
                    // Potentiometers
                    ecu2.pedals.acc_pot1_shared,
                    ecu2.pedals.acc_pot2_shared,
                    ecu2.pedals.brk_pot_shared);
            #else
            if (i != 10)
            {
                body_len = sprintf(
                    body,
                    JSON_1,
                    stest,
                    ecu2.bms_lv[0], // bms lv voltages and temperatures
                    ecu2.bms_lv[1],
                    ecu2.bms_lv[2],
                    ecu2.bms_lv[3],
                    ecu2.bms_lv[4],
                    ecu2.bms_lv[5],
                    ecu2.bms_lv[6],
                    ecu2.bms_lv[7],
                    // FL motor status and actual velocity
                    (ecu2.motorVal1[0].AMK_bSystemReady << 7 &
                     ecu2.motorVal1[0].AMK_bError << 6 &
                     ecu2.motorVal1[0].AMK_bWarn << 5 &
                     ecu2.motorVal1[0].AMK_bQuitDcOn << 4 &
                     ecu2.motorVal1[0].AMK_bDcOn << 3 &
                     ecu2.motorVal1[0].AMK_bQuitInverterOn << 2 &
                     ecu2.motorVal1[0].AMK_bInverterOn << 1 &
                     ecu2.motorVal1[0].AMK_bDerating),
                    ecu2.motorVal1[0].AMK_ActualVelocity,
                    // FR motor status and actual velocity
                    (ecu2.motorVal1[1].AMK_bSystemReady << 7 &
                     ecu2.motorVal1[1].AMK_bError << 6 &
                     ecu2.motorVal1[1].AMK_bWarn << 5 &
                     ecu2.motorVal1[1].AMK_bQuitDcOn << 4 &
                     ecu2.motorVal1[1].AMK_bDcOn << 3 &
                     ecu2.motorVal1[1].AMK_bQuitInverterOn << 2 &
                     ecu2.motorVal1[1].AMK_bInverterOn << 1 &
                     ecu2.motorVal1[1].AMK_bDerating),
                    ecu2.motorVal1[1].AMK_ActualVelocity,
                    // RR motor status and actual velocity
                    (ecu2.motorVal1[2].AMK_bSystemReady << 7 &
                     ecu2.motorVal1[2].AMK_bError << 6 &
                     ecu2.motorVal1[2].AMK_bWarn << 5 &
                     ecu2.motorVal1[2].AMK_bQuitDcOn << 4 &
                     ecu2.motorVal1[2].AMK_bDcOn << 3 &
                     ecu2.motorVal1[2].AMK_bQuitInverterOn << 2 &
                     ecu2.motorVal1[2].AMK_bInverterOn << 1 &
                     ecu2.motorVal1[2].AMK_bDerating),
                    ecu2.motorVal1[2].AMK_ActualVelocity,
                    // RL motor status and actual velocity
                    (ecu2.motorVal1[3].AMK_bSystemReady << 7 &
                     ecu2.motorVal1[3].AMK_bError << 6 &
                     ecu2.motorVal1[3].AMK_bWarn << 5 &
                     ecu2.motorVal1[3].AMK_bQuitDcOn << 4 &
                     ecu2.motorVal1[3].AMK_bDcOn << 3 &
                     ecu2.motorVal1[3].AMK_bQuitInverterOn << 2 &
                     ecu2.motorVal1[3].AMK_bInverterOn << 1 &
                     ecu2.motorVal1[3].AMK_bDerating),
                    ecu2.motorVal1[3].AMK_ActualVelocity,
                    // Torque limits
                    ecu2.motorSetP[0].AMK_TorqueLimitPositive,
                    ecu2.motorSetP[0].AMK_TorqueLimitNegative,
                    ecu2.motorSetP[1].AMK_TorqueLimitPositive,
                    ecu2.motorSetP[1].AMK_TorqueLimitNegative,
                    ecu2.motorSetP[2].AMK_TorqueLimitPositive,
                    ecu2.motorSetP[2].AMK_TorqueLimitNegative,
                    ecu2.motorSetP[3].AMK_TorqueLimitPositive,
                    ecu2.motorSetP[3].AMK_TorqueLimitNegative,
                    // Driver inputs
                    ecu2.status.throttle_shared,
                    ecu2.status.steering_shared,
                    ecu2.status.brake_shared,
                    ecu2.status.actualVelocityKMH_shared,
                    // Most important bms hv data
                    ecu2.bms.max_bms_voltage_shared,
                    ecu2.bms.min_bms_voltage_shared,
                    ecu2.bms.mean_bms_voltage_shared,
                    ecu2.bms.max_bms_temp_shared,
                    ecu2.bms.min_bms_temp_shared,
                    ecu2.bms.mean_bms_temp_shared,
                    // Power info
                    ecu2.power.batteryPack_voltage_shared,
                    ecu2.power.lem_current_shared,
                    // IMU and SMU
                    ecu2.imu.accelerations_shared[0],
                    ecu2.imu.accelerations_shared[1],
                    ecu2.imu.accelerations_shared[2],
                    ecu2.imu.omegas_shared[0],
                    ecu2.imu.omegas_shared[1],
                    ecu2.imu.omegas_shared[2],
                    ecu2.imu.suspensions_shared[0],
                    ecu2.imu.suspensions_shared[1],
                    ecu2.imu.suspensions_shared[2],
                    ecu2.imu.suspensions_shared[3]);
            }
            else
            { // Other data (less important ones)
                body_len = sprintf(
                    body,
                    JSON_2,
                    stest,
                    // FL motor
                    ecu2.motorVal1[0].AMK_TorqueCurrent,
                    ecu2.motorVal1[0].AMK_Voltage,
                    ecu2.motorVal1[0].AMK_Current,
                    // FR motor
                    ecu2.motorVal1[1].AMK_TorqueCurrent,
                    ecu2.motorVal1[1].AMK_Voltage,
                    ecu2.motorVal1[1].AMK_Current,
                    // RR motor
                    ecu2.motorVal1[2].AMK_TorqueCurrent,
                    ecu2.motorVal1[2].AMK_Voltage,
                    ecu2.motorVal1[2].AMK_Current,
                    // RL motor
                    ecu2.motorVal1[3].AMK_TorqueCurrent,
                    ecu2.motorVal1[3].AMK_Voltage,
                    ecu2.motorVal1[3].AMK_Current,
                    // FL motor values
                    ecu2.motorVal2[0].AMK_TempMotor,
                    ecu2.motorVal2[0].AMK_TempInverter,
                    ecu2.motorVal2[0].AMK_TempIGBT,
                    ecu2.motorVal2[0].AMK_ErrorInfo,
                    // FR motor values
                    ecu2.motorVal2[1].AMK_TempMotor,
                    ecu2.motorVal2[1].AMK_TempInverter,
                    ecu2.motorVal2[1].AMK_TempIGBT,
                    ecu2.motorVal2[1].AMK_ErrorInfo,
                    // RR motor values
                    ecu2.motorVal2[2].AMK_TempMotor,
                    ecu2.motorVal2[2].AMK_TempInverter,
                    ecu2.motorVal2[2].AMK_TempIGBT,
                    ecu2.motorVal2[2].AMK_ErrorInfo,
                    // RL motor values
                    ecu2.motorVal2[3].AMK_TempMotor,
                    ecu2.motorVal2[3].AMK_TempInverter,
                    ecu2.motorVal2[3].AMK_TempIGBT,
                    ecu2.motorVal2[3].AMK_ErrorInfo,
                    // Other driver inputs
                    ecu2.status.brakePress_shared,
                    ecu2.status.status_shared,
                    ecu2.pedals.brk_req_shared,
                    ecu2.pedals.throttle_req_shared,
                    // Bms hv error map
                    ecu2.bms.bms_bitmap_shared,
                    // Other power values
                    ecu2.power.curr_sens_shared,
                    ecu2.power.total_power_shared,
                    // Fan data
                    ecu2.fanSpeed.rightFanSpeed_shared,
                    ecu2.fanSpeed.leftFanSpeed_shared,
                    // Pre/post motor/inverter values - if someone (culi) asks, we don't know which one is what :)
                    ecu2.imu.temperatures_shared[0],
                    ecu2.imu.temperatures_shared[1],
                    ecu2.imu.temperatures_shared[2],
                    ecu2.imu.temperatures_shared[3],
                    ecu2.imu.temperatures_shared[4],
                    ecu2.imu.temperatures_shared[5],
                    ecu2.imu.temperatures_shared[6],
                    ecu2.imu.temperatures_shared[7],
                    // Errors
                    ecu2.gpio.Bms_shared,
                    ecu2.gpio.Imd_shared,
                    ecu2.bms.max_bms_temp_nslave_shared,
                    // Potentiometers
                    ecu2.pedals.acc_pot1_shared,
                    ecu2.pedals.acc_pot2_shared,
                    ecu2.pedals.brk_pot_shared);

                // Reset counter
                i = -1;
            }
            #endif

            #ifndef WITH_DEFLATE
            // GZIP compression - tnx chat GPT
            // Length of the input string
            //size_t input_length = body_len + 1; // Include null terminator
            // Allocate memory for the compressed data (maximum possible size)
            //size_t max_compressed_length = compressBound(input_length);
            //unsigned char *compressed_data = (unsigned char *)malloc(max_compressed_length);
            //if (compressed_data == NULL)
            //{
            //    ESP_LOGE("GZIP", "Memory allocation failed.");
            //    xSemaphoreGive(can_insert);
            //    continue;
            //}
            // Compress the input string
            //int ret;
            //if ((ret = compress(compressed_data, &max_compressed_length, (unsigned char *)body, input_length)) != Z_OK)
            //{
            //    ESP_LOGE("GZIP", "Compression failed %i", ret);
            //    xSemaphoreGive(can_insert);
            //    free(compressed_data);
            //    continue;
            //}

            z_stream stream;
            stream.zalloc = (alloc_func)0;
            stream.zfree = (free_func)0;
            stream.opaque = (voidpf)0;

            // 25 = 9 (windowSize) + 16 (gzip header)
            // 7 = memLevel (if greater crash)
            int err = deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 25, 5, Z_DEFAULT_STRATEGY);

            size_t input_length = body_len + 1; // Include null terminator
            size_t max_compressed_length = compressBound(input_length);
            int comprLen = 0;
            unsigned char *compressed_data = (unsigned char *)malloc(max_compressed_length);

            stream.next_out = compressed_data;
            stream.avail_out = 0;
            stream.next_in = (z_const Bytef *) body;
            stream.avail_in = 0;

            const uInt max = (uInt)-1;
            uLong left;
            do {
                if (stream.avail_out == 0) {
                    stream.avail_out = left > (uLong)max ? max : (uInt)left;
                    left -= stream.avail_out;
                }
                if (stream.avail_in == 0) {
                    stream.avail_in = body_len > (uLong)max ? max : (uInt)body_len;
                    body_len -= stream.avail_in;
                }
                err = deflate(&stream, body_len ? Z_NO_FLUSH : Z_FINISH);
            } while (err == Z_OK);

            comprLen = stream.total_out;
            deflateEnd(&stream);

            size_t size = comprLen + comprLen/3 + 5;
            char *encoded_str = (char *)malloc(size);
            size_t encoded_len = 0;
            int base64_ret = mbedtls_base64_encode((unsigned char *) encoded_str, size, &encoded_len, compressed_data, comprLen);
            if (base64_ret != 0) {
                ESP_LOGE("BASE64", "Encoding failed, error code: %d", base64_ret);
            }

            //compressed_data[comprLen] = '\0';
            //ESP_LOGI("", "%s", encoded_str);

            // Free memory

            #ifndef BASE64
            esp_http_client_set_header(http_client, "Content-Encoding", "gzip");
            char http_request_len[10];
            esp_http_client_set_header(http_client, "Content-Length", itoa(encoded_len, http_request_len, 10));
            esp_http_client_set_post_field(http_client, &encoded_str, encoded_len);
            esp_http_client_perform(http_client);
            free(encoded_str);
            #else
            esp_http_client_set_header(http_client, "Content-Encoding", "gzip");
            char http_request_len[10];
            esp_http_client_set_header(http_client, "Content-Length", itoa(comprLen, http_request_len, 10));
            esp_http_client_set_post_field(http_client, &compressed_data, comprLen);
            esp_http_client_perform(http_client);
            free(compressed_data);
            #endif


            #else
            esp_http_client_set_post_field(http_client, body, body_len);
            esp_http_client_perform(http_client);
            #endif

            ESP_LOGI("DB DONE", "JSON_%i inserting done", i == -1 ? 2 : 1);
            // Increment JSON counter
            i++;
            memset(body, 0, BODY_MAX_SIZE);
            xSemaphoreGive(can_insert);
        }
        else
        {
            #ifndef DEBUG
            char* error_type = !connected ? "ESP not connected" : (!uart_cond ? "UART not received" : "Semaphore not taken");
            ESP_LOGE("DB if ERROR", "%s", error_type);
            #endif
            gpio_set_level(DB_ERROR_GPIO, 1);
        }
    }
}
