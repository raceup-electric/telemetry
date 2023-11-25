#include <stdio.h>
#include "supabasec.h"

void supabase_insert()
{
  char *body = (char *)malloc(24500);
  while (1)
  {
    if (body != NULL && xQueueReceive(supabase_q, &ecu2, (TickType_t)10) == pdPASS)
    {
      if (xSemaphoreTake(can_insert, 1) == pdTRUE)
      {
        if (connected)
        { 
          // Controllare decimali
          int body_len = sprintf(body, "{\
              \"stest\": \"%lli\", \
              \
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
                                 stest,
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
        }
        else
          ESP_LOGI("C", "Queue working, Sempahore taken, but not connected");

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
