#ifndef GLOBAL_DEFINITIONS_H
#define GLOBAL_DEFINITIONS_H

#include <stdint.h>
//---------------------------------------------------------
//Select the operational mode of the receiving parsing
//----------------------------------------------------------
//#define LORA_STRING_MODE    //Define that indicates we are receiving strings with LoRa
#define LORA_BYTE_MODE  //Indicates receiving packet id+value(1+4 bytes)
 //#define PRINT_DEBUG
//---------------------------------------------------------
//Useful values to be initialized
//---------------------------------------------------------
#define LORA_IMPLICIT_LENGTH 5

extern struct LoRa_Log logged_values;


struct motors{
    float TempMotor;
    float TempIGBT;
    float TempInverter;
    uint8_t status;
};

struct HV{
    float max_voltage;
    float min_voltage;
    float avg_voltage;
    float max_temp;
    float min_temp;
    float avg_temp;
};

struct LV{
    float min_voltage;
    float tot_voltage;
    float max_temp;
};

struct Temps{
    uint16_t precold;
    uint16_t postcold;
    uint16_t premot;
    uint16_t postmot;
};

struct LoRa_Log {
    int RSSI;
    float SNR;
    uint32_t status;
    struct motors motor_log[4];
    struct HV hv_log;
    struct LV lv_log;
    struct Temps temp_dx_log;
    struct Temps temp_sx_log;
    uint32_t errors;
    uint32_t totPackets;
};
#endif
