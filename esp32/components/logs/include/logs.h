#ifndef LOGS_H_
#define LOGS_H_

struct motorValues1
{
  uint16_t AMK_bSystemReady;    // bool
  uint16_t AMK_bError;          // bool
  uint16_t AMK_bWarn;           // bool
  uint16_t AMK_bQuitDcOn;       // bool
  uint16_t AMK_bDcOn;           // bool
  uint16_t AMK_bQuitInverterOn; // bool
  uint16_t AMK_bInverterOn;     // bool
  uint16_t AMK_bDerating;       // bool

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
  uint16_t AMK_ErrorInfo; // unsigned int
};

struct motorSetPoints
{
  uint16_t AMK_bInverterOn; // bool
  uint16_t AMK_bDcOn;       // bool
  uint16_t AMK_bEnable;     // bool
  uint16_t AMK_bErrorReset; // bool

  int16_t AMK_TargetVelocity;      // int
  int16_t AMK_TorqueLimitPositive; // int
  int16_t AMK_TorqueLimitNegative; // int
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
  int16_t throttle_shared;   // int
  int16_t steering_shared;   // int
  int16_t brake_shared;      // int
  int16_t brakePress_shared; // int
  uint32_t status_shared;
  int16_t actualVelocityKMH_shared; // int
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
  int16_t brk_req_shared;      // int
  int16_t throttle_req_shared; // int

  uint16_t acc1_low_calibration;
  uint16_t acc2_low_calibration;
  uint16_t acc1_high_calibration;
  uint16_t acc2_high_calibration;
  uint16_t brk_low_calibration;
  uint16_t brk_high_calibration;
};

struct Power_Setup_Log
{
  int16_t max_speed_shared; // int
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

#endif