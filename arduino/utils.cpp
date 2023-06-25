#include "utils.h"

extern struct LoRa_Log logged_values;
//---------------------------------------------------------
//Function to convert the received value in bytes to float
//---------------------------------------------------------

float uint32_to_float(uint32_t value){
    union
    {
        uint32_t value;
        float f;
    } temp;
    temp.value=value;

    return temp.f;
}

//---------------------------------------------------------
//Function to convert received buffer to uint32_t
//---------------------------------------------------------
uint32_t buffer_to_uint32(uint8_t bytes[LORA_IMPLICIT_LENGTH]){
    uint32_t temp = 0;
    int i;
    for(i = 1; i < LORA_IMPLICIT_LENGTH; i++){
        temp |= (((uint32_t)bytes[i]) << ((i-1)*8));
    }
    return temp;
}
//---------------------------------------------------------
//Function to convert received buffer and load into the struct
//---------------------------------------------------------
void parse_to_struct(uint8_t received_buffer[LORA_IMPLICIT_LENGTH]){
    uint8_t id = received_buffer[0];
    float received_value = uint32_to_float(buffer_to_uint32(received_buffer));
    switch(id){
        //Reception of motors temperatures
        case 1:
            logged_values.motor_log[0].TempMotor = received_value;
        break;
        case 2:
            logged_values.motor_log[0].TempIGBT = received_value;
        break;
        case 3:
            logged_values.motor_log[0].TempInverter = received_value;
        break;
        case 4:
            logged_values.motor_log[1].TempMotor = received_value;
        break;
        case 5:
            logged_values.motor_log[1].TempIGBT = received_value;
        break;
        case 6:
            logged_values.motor_log[1].TempInverter = received_value;
        break;
        case 7:
            logged_values.motor_log[2].TempMotor = received_value;
        break;
        case 8:
            logged_values.motor_log[2].TempIGBT = received_value;
        break;
        case 9:
            logged_values.motor_log[2].TempInverter = received_value;
        break;
        case 10:
            logged_values.motor_log[3].TempMotor = received_value;
        break;
        case 11:
            logged_values.motor_log[3].TempIGBT = received_value;
        break;
        case 12:
            logged_values.motor_log[3].TempInverter = received_value;
        break;
        //Parsing of the sizes
        case 13:
            logged_values.motor_log[0].status = received_buffer[1];
            logged_values.motor_log[1].status = received_buffer[2];
            logged_values.motor_log[2].status = received_buffer[3];
            logged_values.motor_log[3].status = received_buffer[4];
        break;
        case 14:
            logged_values.status = received_value;
        break;
        case 15:
            logged_values.hv_log.max_voltage = received_value;
        break;
        case 16:
            logged_values.hv_log.min_voltage = received_value;
        break;
        case 17:
            logged_values.hv_log.avg_voltage = received_value;
        break;
        case 18:
            logged_values.hv_log.max_temp = received_value;
        break;
        case 19:
            logged_values.hv_log.min_temp = received_value;
        break;
        case 20:
            logged_values.hv_log.avg_temp = received_value;
        break;
        case 21:
            logged_values.lv_log.min_voltage = received_value;
        break;
        case 22:
            logged_values.lv_log.tot_voltage = received_value;
        break;
        case 23:
            logged_values.lv_log.max_temp = received_value;
        break;
        case 24:
            logged_values.temp_dx_log.precold = received_value;
        break;
        case 25:
            logged_values.temp_dx_log.postcold = received_value;
        break;
        case 26:
            logged_values.temp_dx_log.premot = received_value;
        break;
        case 27:
            logged_values.temp_dx_log.postmot = received_value;
        break;
        case 28:
            logged_values.temp_sx_log.precold = received_value;
        break;
        case 29:
            logged_values.temp_sx_log.postcold = received_value;
        break;
        case 30:
            logged_values.temp_sx_log.premot = received_value;
        break;
        case 31:
            logged_values.temp_sx_log.postmot = received_value;
        break;
        
    }
}
