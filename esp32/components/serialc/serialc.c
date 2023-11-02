#include <stdio.h>
#include "serialc.h"

void serial_init(void)
{
  uart_config_t uart_config = {
      .baud_rate = UART_BAUD,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
      .source_clk = UART_SCLK_DEFAULT,
  };
  int intr_alloc_flags = 0;

  ESP_ERROR_CHECK(uart_driver_install(UART_NUM, BUF_SIZE, BUF_SIZE, 10, &uart_queue, intr_alloc_flags));
  ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_config));
  ESP_ERROR_CHECK(uart_set_pin(UART_NUM, UART_TX, UART_RX, UART_RTS, UART_CTS));
  ESP_ERROR_CHECK(uart_enable_pattern_det_baud_intr(UART_NUM, '\0', 1, 10, 100, 0));
  ESP_ERROR_CHECK(uart_pattern_queue_reset(UART_NUM, 2));
}

void serial_receive()
{
  char *data = (char *)malloc(BUF_SIZE);

  uart_event_t uart_event;
  while (1)
  {
    if (xQueueReceive(uart_queue, (void *)&uart_event, portTICK_PERIOD_MS))
    {
      switch (uart_event.type)
      {
      case UART_PATTERN_DET:
      {
        int pattern_pos = uart_pattern_pop_pos(UART_NUM);
        int read_len = uart_read_bytes(UART_NUM, data, pattern_pos, pdMS_TO_TICKS(100));
        uart_flush_input(UART_NUM);

        if (read_len - 1 == sizeof(struct logs))
        {
          uint8_t decoded[read_len - 1];
          cobs_decode(decoded, sizeof(decoded), data, read_len);
          memcpy(&ecu, &decoded, sizeof(decoded));

          xQueueSend(supabase_q, &ecu, 0);

          memset(data, 0, BUF_SIZE);
        }
      }
      default:
        break;
      }
    }
  }
}
