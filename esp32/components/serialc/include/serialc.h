#ifndef SERIALC_H_
#define SERIALC_H_

// include logs structs, global definitions and cobs
#include "structures.h"
#include "globals.h"
#include "cobs.h"
extern struct logs ecu;

// esp32 drivers and utils
#include <string.h>
#include "driver/uart.h"
#include "driver/gpio.h"

// uart definitions
#define UART_RTS UART_PIN_NO_CHANGE
#define UART_CTS UART_PIN_NO_CHANGE
#define BUF_SIZE (2048)

extern QueueHandle_t ecu_data;

void serial_init(void);

void serial_receive();

#endif