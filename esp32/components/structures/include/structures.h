#ifndef LOGS_H_
#define LOGS_H_

struct motorValues1
{
  uint16_t AMK_bSystemReady;                // bool in TI
  uint16_t AMK_bError;                      // bool in TI
  uint16_t AMK_bWarn;                       // bool in TI
  uint16_t AMK_bQuitDcOn;                   // bool in TI
  uint16_t AMK_bDcOn;                       // bool in TI
  uint16_t AMK_bQuitInverterOn;             // bool in TI
  uint16_t AMK_bInverterOn;                 // bool in TI
  uint16_t AMK_bDerating;                   // bool in TI

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
  uint16_t AMK_ErrorInfo;                   // unsigned int in TI
};

struct motorSetPoints
{
  uint16_t AMK_bInverterOn;                 // bool in TI
  uint16_t AMK_bDcOn;                       // bool in TI
  uint16_t AMK_bEnable;                     // bool in TI
  uint16_t AMK_bErrorReset;                 // bool in TI

  int16_t AMK_TargetVelocity;               // int in TI
  int16_t AMK_TorqueLimitPositive;          // int in TI
  int16_t AMK_TorqueLimitNegative;          // int in TI
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
  int16_t throttle_shared;                  // int in TI
  int16_t steering_shared;                  // int in TI
  int16_t brake_shared;                     // int in TI
  int16_t brakePress_shared;                // int in TI
  uint32_t status_shared;
  int16_t actualVelocityKMH_shared;         // int in TI
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
  int16_t brk_req_shared;                   // int in TI
  int16_t throttle_req_shared;              // int in TI

  uint16_t acc1_low_calibration;
  uint16_t acc2_low_calibration;
  uint16_t acc1_high_calibration;
  uint16_t acc2_high_calibration;
  uint16_t brk_low_calibration;
  uint16_t brk_high_calibration;
};

struct Power_Setup_Log
{
  int16_t max_speed_shared;                 // int in TI
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
};

// oneJSON
#define JSON "{\
  \"stest\": %lli, \
  \"timestamp\": %lli,\
  \"bms_lv0\": %.2f, \
  \"bms_lv1\": %.2f, \
  \"bms_lv2\": %.2f, \
  \"bms_lv3\": %.2f, \
  \"bms_lv4\": %.2f, \
  \"bms_lv5\": %.2f, \
  \"bms_lv6\": %.2f, \
  \"bms_lv7\": %.2f, \
  \
  \"amk_status_fl\": %i, \
  \"amk_actual_velocity_fl\": %.2f, \
  \
  \"amk_status_fr\": %i, \
  \"amk_actual_velocity_fr\": %.2f, \
  \
  \"amk_status_rr\": %i, \
  \"amk_actual_velocity_rr\": %.2f, \
  \
  \"amk_status_rl\": %i, \
  \"amk_actual_velocity_rl\": %.2f, \
  \
  \"amk_torque_limit_positive_fl\": %i, \
  \"amk_torque_limit_negative_fl\": %i, \
  \"amk_torque_limit_positive_fr\": %i, \
  \"amk_torque_limit_negative_fr\": %i, \
  \"amk_torque_limit_positive_rr\": %i, \
  \"amk_torque_limit_negative_rr\": %i, \
  \"amk_torque_limit_positive_rl\": %i, \
  \"amk_torque_limit_negative_rl\": %i, \
  \
  \"throttle\": %i, \
  \"steering_angle\": %i, \
  \"brake\": %i, \
  \"actual_velocity_kmh\": %i, \
  \
  \"max_hv_volt\": %.2f, \
  \"min_hv_volt\": %.2f, \
  \"avg_hv_volt\": %.2f, \
  \"max_hv_temp\": %.2f, \
  \"min_hv_temp\": %.2f, \
  \"avg_hv_temp\": %.2f, \
  \
  \"car_voltage\": %.2f, \
  \"lem_current\": %.2f, \
  \
  \"acceleration_x\": %.2f, \
  \"acceleration_y\": %.2f, \
  \"acceleration_z\": %.2f, \
  \"omega_x\": %.2f, \
  \"omega_y\": %.2f, \
  \"omega_z\": %.2f, \
  \
  \"suspensions_rl\": %.2f, \
  \"suspensions_fl\": %.2f, \
  \"suspensions_fr\": %.2f, \
  \"suspensions_rr\": %.2f, \
  \
  \"amk_torque_current_fl\": %.2f, \
  \"amk_voltage_fl\": %i, \
  \"amk_current_fl\": %.2f, \
  \
  \"amk_torque_current_fr\": %.2f, \
  \"amk_voltage_fr\": %i, \
  \"amk_current_fr\": %.2f, \
  \
  \"amk_torque_current_rr\": %.2f, \
  \"amk_voltage_rr\": %i, \
  \"amk_current_rr\": %.2f, \
  \
  \"amk_torque_current_rl\": %.2f, \
  \"amk_voltage_rl\": %i, \
  \"amk_current_rl\": %.2f, \
  \
  \"amk_temp_motor_fl\": %.2f, \
  \"amk_temp_inverter_fl\": %.2f, \
  \"amk_temp_igbt_fl\": %.2f, \
  \"amk_error_info_fl\": %i, \
  \
  \"amk_temp_motor_fr\": %.2f, \
  \"amk_temp_inverter_fr\": %.2f, \
  \"amk_temp_igbt_fr\": %.2f, \
  \"amk_error_info_fr\": %i, \
  \
  \"amk_temp_motor_rr\": %.2f, \
  \"amk_temp_inverter_rr\": %.2f, \
  \"amk_temp_igbt_rr\": %.2f, \
  \"amk_error_info_rr\": %i, \
  \
  \"amk_temp_motor_rl\": %.2f, \
  \"amk_temp_inverter_rl\": %.2f, \
  \"amk_temp_igbt_rl\": %.2f, \
  \"amk_error_info_rl\": %i, \
  \
  \"brake_press\": %i, \
  \"car_status\": \"%lu\", \
  \"brk_req\": %i, \
  \"thr_req\": %i, \
  \
  \"bms_error_map\": %i, \
  \
  \"current_sens\": %.2f, \
  \"total_power\": %.2f, \
  \
  \"fan_speed_r\": %i, \
  \"fan_speed_l\": %i, \
  \
  \"temp_inv_dx_2\": %.2f, \
  \"temp_inv_sx_1\": %.2f, \
  \"temp_inv_dx_1\": %.2f, \
  \"temp_inv_sx_2\": %.2f, \
  \"temp_mot_rr_1\": %.2f, \
  \"temp_mot_rl_1\": %.2f, \
  \"temp_mot_rl_2\": %.2f, \
  \"temp_mot_rr_2\": %.2f, \
  \
  \"gpio_bms\": %i, \
  \"gpio_imd\": %i, \
  \"max_temp_n_slave\": %i, \
  \
  \"acc_pot_1\": %.2f, \
  \"acc_pot_2\": %.2f, \
  \"brk_pot\": %.2f \
}"

// first json for very important data
#define JSON_1 "{\
  \"stest\": %lli, \
  \"timestamp\": %lli,\
  \
  \"bms_lv0\": %.2f, \
  \"bms_lv1\": %.2f, \
  \"bms_lv2\": %.2f, \
  \"bms_lv3\": %.2f, \
  \"bms_lv4\": %.2f, \
  \"bms_lv5\": %.2f, \
  \"bms_lv6\": %.2f, \
  \"bms_lv7\": %.2f, \
  \
  \"amk_status_fl\": %i, \
  \"amk_actual_velocity_fl\": %.2f, \
  \
  \"amk_status_fr\": %i, \
  \"amk_actual_velocity_fr\": %.2f, \
  \
  \"amk_status_rr\": %i, \
  \"amk_actual_velocity_rr\": %.2f, \
  \
  \"amk_status_rl\": %i, \
  \"amk_actual_velocity_rl\": %.2f, \
  \
  \"amk_torque_limit_positive_fl\": %i, \
  \"amk_torque_limit_negative_fl\": %i, \
  \"amk_torque_limit_positive_fr\": %i, \
  \"amk_torque_limit_negative_fr\": %i, \
  \"amk_torque_limit_positive_rr\": %i, \
  \"amk_torque_limit_negative_rr\": %i, \
  \"amk_torque_limit_positive_rl\": %i, \
  \"amk_torque_limit_negative_rl\": %i, \
  \
  \"throttle\": %i, \
  \"steering_angle\": %i, \
  \"brake\": %i, \
  \"actual_velocity_kmh\": %i, \
  \
  \"max_hv_volt\": %.2f, \
  \"min_hv_volt\": %.2f, \
  \"avg_hv_volt\": %.2f, \
  \"max_hv_temp\": %.2f, \
  \"min_hv_temp\": %.2f, \
  \"avg_hv_temp\": %.2f, \
  \
  \"car_voltage\": %.2f, \
  \"lem_current\": %.2f, \
  \
  \"acceleration_x\": %.2f, \
  \"acceleration_y\": %.2f, \
  \"acceleration_z\": %.2f, \
  \"omega_x\": %.2f, \
  \"omega_y\": %.2f, \
  \"omega_z\": %.2f, \
  \
  \"suspensions_rl\": %.2f, \
  \"suspensions_fl\": %.2f, \
  \"suspensions_fr\": %.2f, \
  \"suspensions_rr\": %.2f\
}"

// second json for less important data
#define JSON_2 "{\
  \"stest\": \"%lli\", \
  \"timestamp\": %lli,\
  \
  \"amk_torque_current_fl\": %.2f, \
  \"amk_voltage_fl\": %i, \
  \"amk_current_fl\": %.2f, \
  \
  \"amk_torque_current_fr\": %.2f, \
  \"amk_voltage_fr\": %i, \
  \"amk_current_fr\": %.2f, \
  \
  \"amk_torque_current_rr\": %.2f, \
  \"amk_voltage_rr\": %i, \
  \"amk_current_rr\": %.2f, \
  \
  \"amk_torque_current_rl\": %.2f, \
  \"amk_voltage_rl\": %i, \
  \"amk_current_rl\": %.2f, \
  \
  \"amk_temp_motor_fl\": %.2f, \
  \"amk_temp_inverter_fl\": %.2f, \
  \"amk_temp_igbt_fl\": %.2f, \
  \"amk_error_info_fl\": %i, \
  \
  \"amk_temp_motor_fr\": %.2f, \
  \"amk_temp_inverter_fr\": %.2f, \
  \"amk_temp_igbt_fr\": %.2f, \
  \"amk_error_info_fr\": %i, \
  \
  \"amk_temp_motor_rr\": %.2f, \
  \"amk_temp_inverter_rr\": %.2f, \
  \"amk_temp_igbt_rr\": %.2f, \
  \"amk_error_info_rr\": %i, \
  \
  \"amk_temp_motor_rl\": %.2f, \
  \"amk_temp_inverter_rl\": %.2f, \
  \"amk_temp_igbt_rl\": %.2f, \
  \"amk_error_info_rl\": %i, \
  \
  \"brake_press\": %i, \
  \"car_status\": \"%lu\", \
  \"brk_req\": %i, \
  \"thr_req\": %i, \
  \
  \"bms_error_map\": %i, \
  \
  \"current_sens\": %.2f, \
  \"total_power\": %.2f, \
  \
  \"fan_speed_r\": %i, \
  \"fan_speed_l\": %i, \
  \
  \"temp_inv_dx_2\": %.2f, \
  \"temp_inv_sx_1\": %.2f, \
  \"temp_inv_dx_1\": %.2f, \
  \"temp_inv_sx_2\": %.2f, \
  \"temp_mot_rr_1\": %.2f, \
  \"temp_mot_rl_1\": %.2f, \
  \"temp_mot_rl_2\": %.2f, \
  \"temp_mot_rr_2\": %.2f, \
  \
  \"gpio_bms\": %i, \
  \"gpio_imd\": %i, \
  \"max_temp_n_slave\": %i, \
  \
  \"acc_pot_1\": %.2f, \
  \"acc_pot_2\": %.2f, \
  \"brk_pot\": %.2f \
}"

#endif