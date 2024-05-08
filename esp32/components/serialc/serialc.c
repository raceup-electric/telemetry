#include <stdio.h>
#include "serialc.h"
#include "esp_log.h"

static QueueHandle_t uart_queue;

void serial_init(void)
{
  // uart configuration
  uart_config_t uart_config = {
      .baud_rate = UART_BAUD,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
      .source_clk = UART_SCLK_DEFAULT,
  };

  // uart initialization
  ESP_ERROR_CHECK(uart_driver_install(UART_NUM, BUF_SIZE * 2, 512, 50, &uart_queue, 0));
  ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_config));
  ESP_ERROR_CHECK(uart_set_pin(UART_NUM, UART_TX, UART_RX, UART_RTS, UART_CTS));
  ESP_ERROR_CHECK(uart_enable_pattern_det_baud_intr(UART_NUM, '\0', 1, 2000 , 0, 0));
  ESP_ERROR_CHECK(uart_pattern_queue_reset(UART_NUM, 5));
}

void serial_receive()
{
  // reserve memory for incoming bytes
  char data[BUF_SIZE];
  uint8_t decoded[sizeof(struct logs)];
  uart_event_t uart_event;
  while (1)
  {
    // if data in uart queue
    if (xQueueReceive(uart_queue, (void *)&uart_event, portTICK_PERIOD_MS) == pdTRUE)
    {
      switch (uart_event.type)
      {
        case UART_FIFO_OVF:
        {
          ESP_LOGW("UART", "HW FIFO full");
          uart_flush_input(UART_NUM);
          xQueueReset(uart_queue);
          break;
        }
        case UART_BUFFER_FULL: 
        {  
          ESP_LOGW("UART", "Ring buffer full");
          uart_flush_input(UART_NUM);
          xQueueReset(uart_queue);
          break;
        }
        // event: detect terminator
        case UART_PATTERN_DET:
        {
          ESP_LOGI("UART", "Pattern detected");
          // int pattern_pos = uart_pattern_pop_pos(UART_NUM);
          // if (pattern_pos < 0) {
          //   // Pattern queue full, flush queue
          //   ESP_LOGW("UART", "Pattern queue full");
          //   uart_flush_input(UART_NUM);
          //   uart_pattern_queue_reset(UART_NUM, 10);
          //   break;
          // }
          // int read_len = uart_read_bytes(UART_NUM, data, pattern_pos, pdMS_TO_TICKS(100));
          // uart_flush_input(UART_NUM);
          // ESP_LOGI("UART", "Read %d bytes", read_len);
          // // check struct completness
          // // COBS adds at least 1 and at most roundup(size/254) (in this case 2) bytes of overhead
          // if (read_len == sizeof(struct logs) + 1 || read_len == sizeof(struct logs) + 2)
          // {
          //    ESP_LOGI("UART", "Decoding COBS");
          //    cobs_decode(decoded, sizeof(decoded), data, read_len);
          //    // send struct to task
          //    xQueueSend(ecu_data, &decoded, 0);
          //  }
          int pos = uart_pattern_pop_pos(UART_NUM);
          ESP_LOGI("UART", "Pattern detected pos: %d", pos);
          if (pos == -1) {
            // There used to be a UART_PATTERN_DET event, but the pattern position queue is full so that it can not
            // record the position. We should set a larger queue size.
            // As an example, we directly flush the rx buffer here.
            ESP_LOGW("UART", "Pattern queue full");
            uart_flush_input(UART_NUM);
            uart_pattern_queue_reset(UART_NUM, 5);
          } 
          else {
            int read = uart_read_bytes(UART_NUM, data, pos, 100 / portTICK_PERIOD_MS);
            ESP_LOGI("UART", "Read %d bytes", read);
            // uart_flush_input(UART_NUM);
            if (read == sizeof(struct logs) + 1 || read == sizeof(struct logs) + 2)
            {
              ESP_LOGI("UART", "Decoding COBS");
              cobs_decode(decoded, sizeof(decoded), data, read);
              // send struct to task
              xQueueSend(ecu_data, &decoded, 0);
            }
          }
          break;
        }
        default:
          ;
      }
    }
  }
}
