#include "json_manager.h"


/*void send_JSON(){
    StaticJsonDocument<1024> doc;

    JsonObject motors = doc.createNestedObject("motors");

    JsonObject motors_FL = motors.createNestedObject("FL");
    motors_FL["TempMotor"] = logged_values.motor_log[0].TempMotor;
    motors_FL["TempIGBT"] = logged_values.motor_log[0].TempIGBT;
    motors_FL["TempInverter"] = logged_values.motor_log[0].TempInverter;
    motors_FL["status"] = logged_values.motor_log[0].status;

    JsonObject motors_FR = motors.createNestedObject("FR");
    motors_FR["TempMotor"] = logged_values.motor_log[1].TempMotor;
    motors_FR["TempIGBT"] = logged_values.motor_log[1].TempIGBT;
    motors_FR["TempInverter"] = logged_values.motor_log[1].TempInverter;
    motors_FR["status"] = logged_values.motor_log[1].status;
    
    JsonObject motors_RL = motors.createNestedObject("RL");
    motors_RL["TempMotor"] = logged_values.motor_log[2].TempMotor;
    motors_RL["TempIGBT"] = logged_values.motor_log[2].TempIGBT;
    motors_RL["TempInverter"] = logged_values.motor_log[2].TempInverter;
    motors_RL["status"] = logged_values.motor_log[2].status;

    JsonObject motors_RR = motors.createNestedObject("RR");
    motors_RR["TempMotor"] = logged_values.motor_log[3].TempMotor;
    motors_RR["TempIGBT"] = logged_values.motor_log[3].TempIGBT;
    motors_RR["TempInverter"] = logged_values.motor_log[3].TempInverter;
    motors_RR["status"] = logged_values.motor_log[3].status;

    JsonObject HV = doc.createNestedObject("HV");
    HV["max_voltage"] = logged_values.hv_log.max_voltage;
    HV["min_voltage"] = logged_values.hv_log.min_voltage;
    HV["avg_voltage"] = logged_values.hv_log.avg_voltage;
    HV["max_temp"] = logged_values.hv_log.max_temp;
    HV["min_temp"] = logged_values.hv_log.min_temp;
    HV["avg_temp"] = logged_values.hv_log.avg_temp;

    JsonObject LV = doc.createNestedObject("LV");
    LV["min_voltage"] = logged_values.lv_log.min_voltage;
    LV["tot_voltage"] = logged_values.lv_log.tot_voltage;
    LV["max_temp"] = logged_values.lv_log.max_temp;

    JsonObject Temps_Dx = doc.createNestedObject("Temps_Dx");
    Temps_Dx["precold"] = logged_values.temp_dx_log.precold;
    Temps_Dx["postcold"] = logged_values.temp_dx_log.postcold;
    Temps_Dx["premot"] = logged_values.temp_dx_log.premot;
    Temps_Dx["postmot"] = logged_values.temp_dx_log.postmot;

    JsonObject Temps_Sx = doc.createNestedObject("Temps_Dx");
    Temps_Sx["precold"] = logged_values.temp_sx_log.precold;
    Temps_Sx["postcold"] = logged_values.temp_sx_log.postcold;
    Temps_Sx["premot"] = logged_values.temp_sx_log.premot;
    Temps_Sx["postmot"] = logged_values.temp_sx_log.postmot;

    serializeJson(doc, Serial);
}*/
