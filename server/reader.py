from serial.threaded import Packetizer
from cobs import cobs
import globals
import time
import struct
from struct import *

FORMAT_PAYLOAD = "i" + "f" + "I" + ("f" * 3 + "B") * 4 + ("f" * 6) + ("f" * 3) + ("H" * 4) * 2 + "I"
size_payload = struct.calcsize(FORMAT_PAYLOAD)

class SerialReader(Packetizer):
  def handle_packet(self, packet: bytes) -> None:
    globals.lock.acquire()
    globals.last_received = time.time()
    # globals.data = []
    try:
      decodedPacket = cobs.decode(packet)
      if len(decodedPacket) <= 21: raise cobs.DecodeError
      data_unpacked = unpack(FORMAT_PAYLOAD, decodedPacket)
      print(data_unpacked)
      globals.data = {
        "RSSI": data_unpacked[0],
        "SNR": data_unpacked[1],
        "car_status": data_unpacked[2],
        "temperature": {
          "motors": {
            "fl": data_unpacked[3],
            "fr": data_unpacked[7],
            "rl": data_unpacked[11],
            "rr": data_unpacked[15],
          },
          "IGBT": {
            "fl": data_unpacked[4],
            "fr": data_unpacked[8],
            "rl": data_unpacked[12],
            "rr": data_unpacked[16],
          },
          "inverter": {
            "fl": data_unpacked[5],
            "fr": data_unpacked[9],
            "rl": data_unpacked[13],
            "rr": data_unpacked[17]
          }
        },
        "status": {
          "motor": {
            "fl": data_unpacked[6],
            "fr": data_unpacked[10],
            "rl": data_unpacked[14],
            "rr": data_unpacked[18]
          }
        },
        "voltage": {
          "hv": {
            "max": data_unpacked[19],
            "min": data_unpacked[20],
            "mean": data_unpacked[21]
          },
          "lv": {
            "min": data_unpacked[25],
            "tot": data_unpacked[26]
          }
        },
        "temp": {
          "hv": {
            "max": data_unpacked[22],
            "min": data_unpacked[23],
            "mean": data_unpacked[24]
          },
          "lv": {
            "max": data_unpacked[27]
          },
          "dx": {
            "precold": data_unpacked[28],
            "postcold": data_unpacked[29],
            "premot": data_unpacked[30],
            "postmot": data_unpacked[31],
          },
          "sx": {
            "precold": data_unpacked[32],
            "postcold": data_unpacked[33],
            "premot": data_unpacked[34],
            "postmot": data_unpacked[35],
          }
        },
        "ERROR_PKT": data_unpacked[36],
      }
      globals.lora_error = False
    except struct.error as e:
      print(e)
      globals.reader_error = True
    except cobs.DecodeError as e:
      print("Starting LoRa failed!")
      globals.lora_error = True
    finally:
      globals.lock.release()
      