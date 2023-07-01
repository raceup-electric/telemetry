import struct
import serial
from struct import *

# H -> half_word (2 Byte),  ? -> bool (1 Byte),  c -> char (1 Byte), I -> Unsigned int

# i -> RSSI
# f -> SNR
# I -> uint32_t status
# (f * 3 + B) * 4 -> motors 
# f * 6 -> HV
# f * 3 -> LV
# (H * 4) * 2 -> Temps 
FORMAT_PAYLOAD = "i" + "f" + "I" + ("f" * 3 + "B") * 4 + ("f" * 6) + ("f" * 3) + ("H" * 4) * 2
size_payload = struct.calcsize(FORMAT_PAYLOAD)

ser = serial.Serial(port="COM15", baudrate=115200, timeout=.1)

while(True):
    ser.flush()

    if(ser.in_waiting == size_payload):
        data = ser.read(size_payload)
        data_unpacked = unpack(FORMAT_PAYLOAD, data)
        print(data_unpacked)
        
