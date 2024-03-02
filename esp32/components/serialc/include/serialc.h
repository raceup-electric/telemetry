#ifndef SERIALC_H_
#define SERIALC_H_

#include <string.h>

#include "driver/uart.h"
#include "driver/gpio.h"

#include "esp_log.h"

#include "logs.h"
extern struct logs ecu;

#include "cobs.h"

extern QueueHandle_t supabase_q;

#define UART_TX GPIO_NUM_17
#define UART_RX GPIO_NUM_18 // esp32 = 16, esp32-s3 = 18
#define UART_RTS UART_PIN_NO_CHANGE
#define UART_CTS UART_PIN_NO_CHANGE

#define UART_NUM UART_NUM_1
#define UART_BAUD 460800 //115200
#define BUF_SIZE (1024)

static QueueHandle_t uart_queue;

void serial_init(void);

void serial_receive();

#endif