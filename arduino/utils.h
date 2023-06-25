#ifndef UTILS_H
#define UTILS_H

#include "global_definitions.h"
#include "stdint.h"

//---------------------------------------------------------
//Function to convert the received value in bytes to float
//---------------------------------------------------------
float uint32_to_float(uint32_t value);

//---------------------------------------------------------
//Function to convert received buffer to uint32_t
//---------------------------------------------------------
uint32_t buffer_to_uint32(uint8_t bytes[LORA_IMPLICIT_LENGTH]);

void parse_to_struct(uint8_t received_buffer[LORA_IMPLICIT_LENGTH]);
#endif
