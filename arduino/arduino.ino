#include <SPI.h>
#include "LoRa_Custom.h"
#include "global_definitions.h"
#include "utils.h"
#include <stdint.h>

struct LoRa_Log logged_values;
int counter = 0;

int failed_packets = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  while (!LoRa.begin(868e6)) {
    Serial.write("Starting LoRa failed!");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(300);
  }

  digitalWrite(LED_BUILTIN, LOW);

  LoRa.setSignalBandwidth(250e3);
  LoRa.setSpreadingFactor(6);
  LoRa.setCodingRate4(4);
  //LoRa.enableCrc();
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket(LORA_IMPLICIT_LENGTH);

  if (packetSize) {
    // received a packet
    #ifdef LORA_STRING_MODE
      String incoming = "";

      while (LoRa.available()) 
        incoming += (char)LoRa.read();
    #endif

    #ifdef LORA_BYTE_MODE
      uint8_t received_buffer[LORA_IMPLICIT_LENGTH];
      int i = 0;

      while(LoRa.available())
        received_buffer[i++] = (uint8_t)LoRa.read();

      parse_to_struct(received_buffer);
      logged_values.RSSI = LoRa.packetRssi();
      logged_values.SNR = LoRa.packetSnr();

      //if(received_buffer[0] == 31)
      Serial.write((const char *)&logged_values, sizeof(logged_values)); 
      Serial.write(0xFF);
      Serial.write(0xFF);
      Serial.write(0xFF);
      Serial.write(0xFF);

      #ifdef PRINT_DEBUG
        Serial.print("ID: ");
        Serial.println(received_buffer[0]);
        Serial.print("Value: ");
        Serial.println(uint32_to_float(buffer_to_uint32(received_buffer)));
        Serial.println("RSSI: " + String(LoRa.packetRssi()));
        Serial.println("SNR: " + String(LoRa.packetSnr()));
      #endif
    #endif
  }

  #ifdef PRINT_DEBUG
    if(counter % 10 == 0){
      //send_JSON();
      Serial.println(LoRa.failed_packets);
      //Serial.write((const char *)&logged_values.temp_dx_log, sizeof(logged_values.temp_dx_log) + 1024); 
      counter = 0;
    }
    counter++;
  #endif
  
  delay(1);
}
