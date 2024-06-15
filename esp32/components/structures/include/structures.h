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

struct Status_Log
{
  int16_t throttle;                  // int in TI
  int16_t steering;                  // int in TI
  int16_t brake;                     // int in TI
  int16_t brakePress;                // int in TI
  int16_t actualVelocityKMH;         // int in TI
  uint16_t status;
};

struct Pedals_Log
{
  float acc_pot;
  float brk_pot;
  int16_t brk_req;                   // int in TI
  int16_t acc_req;                   // int in TI
};

struct BMS_Log
{
  float max_bms_voltage;
  float min_bms_voltage;
  float mean_bms_voltage;
  float max_bms_temp;
  float min_bms_temp;
  float mean_bms_temp;
  uint16_t max_bms_temp_nslave;
  uint16_t bms_bitmap;
};

struct Power_Log
{
  float lem_current;
  float batteryPack_voltage;
  float curr_sens;
  float total_power;
};

struct FanSpeed_Log
{
  uint16_t fanSpeed;
};

struct Imu_Log
{
  float accelerations[3];
  float omegas[3];
  float temperatures[10];
  float suspensions[4];
};

struct Gpio_Log
{
  uint16_t Imd;
  uint16_t Bms;
};

struct Power_Setup_Log
{
  int16_t max_speed;                 // int in TI
  float rear_motor_scale;
  float front_motor_scale;
  float power_limit;
};

struct GPS {
  uint16_t lap;
  float velocity;
  float lati;
  float longi;
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
  struct GPS gps;
};

// oneJSON

#define JSON "{\
  \"stest\": %lli, \
  \"timestamp\": %lli,\
  \"lap\": %i,\
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
  \"amk_torque_current_fl\": %.2f, \
  \"amk_voltage_fl\": %i, \
  \"amk_current_fl\": %.2f, \
  \
  \"amk_status_fr\": %i, \
  \"amk_actual_velocity_fr\": %.2f, \
  \"amk_torque_current_fr\": %.2f, \
  \"amk_voltage_fr\": %i, \
  \"amk_current_fr\": %.2f, \
  \
  \"amk_status_rr\": %i, \
  \"amk_actual_velocity_rr\": %.2f, \
  \"amk_torque_current_rr\": %.2f, \
  \"amk_voltage_rr\": %i, \
  \"amk_current_rr\": %.2f, \
  \
  \"amk_status_rl\": %i, \
  \"amk_actual_velocity_rl\": %.2f, \
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
  \"brake_press\": %i, \
  \"actual_velocity_kmh\": %i, \
  \"car_status\": \"%i\", \
  \
  \"acc_pot\": %.2f, \
  \"brk_pot\": %.2f, \
  \"brk_req\": %i, \
  \"thr_req\": %i, \
  \
  \"max_hv_volt\": %.2f, \
  \"min_hv_volt\": %.2f, \
  \"avg_hv_volt\": %.2f, \
  \"max_hv_temp\": %.2f, \
  \"min_hv_temp\": %.2f, \
  \"avg_hv_temp\": %.2f, \
  \"max_temp_n_slave\": %i, \
  \"bms_error_map\": %i, \
  \
  \"lem_current\": %.2f, \
  \"car_voltage\": %.2f, \
  \"current_sens\": %.2f, \
  \"total_power\": %.2f, \
  \
  \"fan_speed\": %i, \
  \
  \"acceleration_x\": %.2f, \
  \"acceleration_y\": %.2f, \
  \"acceleration_z\": %.2f, \
  \"omega_x\": %.2f, \
  \"omega_y\": %.2f, \
  \"omega_z\": %.2f, \
  \"cplate_post_rl\": %.2f, \
  \"cplate_post_rr\": %.2f, \
  \"cplate_pre_rl\": %.2f, \
  \"cplate_pre_rr\": %.2f, \
  \"motor_post_rl\": %.2f, \
  \"motor_post_rr\": %.2f, \
  \"motor_pre_rl\": %.2f, \
  \"motor_pre_rr\": %.2f, \
  \"suspensions_rl\": %.2f, \
  \"suspensions_fl\": %.2f, \
  \"suspensions_fr\": %.2f, \
  \"suspensions_rr\": %.2f, \
  \
  \"gpio_bms\": %i, \
  \"gpio_imd\": %i, \
  \
  \"velocity\": %.2f, \
  \"latitude\": %.2f, \
  \"longitude\": %.2f \
}"

#endif