#include <stdio.h>
#include "supabasec.h"

#define MINIZ_NO_ZLIB_APIS

void supabase_insert()
{
    //char body[BODY_MAX_SIZE];
    int i = 0; // JSON (select) counter
    char *body = (char *) malloc(BODY_MAX_SIZE);
    // continuous task
    while (1)
    {
        // check if connected to wifi, struct arrived and semaphore taken
        if (connected)
        {
            if (xQueueReceive(supabase_q, &ecu2, (TickType_t)10) == pdPASS)
            {
                if (xSemaphoreTake(can_insert, 1) == pdTRUE)
                {
                    int body_len = 0;
                    // Most important data
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
                            ecu2.status.actualVelocityKMH_shared,
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

                    // GZIP compression -tnx chat GPT
                    // Length of the input string
                    size_t input_length = body_len + 1; // Include null terminator
                    // Allocate memory for the compressed data (maximum possible size)
                    size_t max_compressed_length = compressBound(input_length);
                    unsigned char *compressed_data = (unsigned char *) malloc(max_compressed_length);
                    if (compressed_data == NULL)
                    {
                        ESP_LOGE("GZIP", "Memory allocation failed.");
                        xSemaphoreGive(can_insert);
                        continue;
                    }
                    // Compress the input string
                    int ret;
                    if ((ret = compress(compressed_data, &max_compressed_length, (unsigned char *)body, input_length)) != Z_OK)
                    {
                        ESP_LOGE("GZIP", "Compression failed %i", ret);
                        xSemaphoreGive(can_insert);
                        free(compressed_data);
                        continue;
                    }
                    // Free memory

                    esp_http_client_set_post_field(http_client, &compressed_data, strlen(&compressed_data));
                    esp_http_client_perform(http_client);

                    free(compressed_data);

                    ESP_LOGI("DB DONE", "JSON_%i inserting done", i == -1 ? 2 : 1);
                    // Increment JSON counter
                    i++;
                    memset(body, 0, BODY_MAX_SIZE);
                    xSemaphoreGive(can_insert);
                }
                else
                    ESP_LOGD("DB DEBUG", "Semaphore not taken");
            }
            else
                ESP_LOGD("DB DEBUG", "UART not received");
        }
        else
            ESP_LOGW("DB WARNING", "ESP not connected");
    }
}
