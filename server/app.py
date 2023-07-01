import serial
from serial.threaded import ReaderThread
from flask import Flask
from flask_apscheduler import APScheduler
from flask_cors import CORS
from flask_socketio import SocketIO
from reader import SerialReader
import globals

import struct
from struct import *
# i -> RSSI
# f -> SNR
# I -> uint32_t status
# (f * 3 + B) * 4 -> motors 
# f * 6 -> HV
# f * 3 -> LV
# (H * 4) * 2 -> Temps 
FORMAT_PAYLOAD = "i" + "f" + "I" + ("f" * 3 + "B") * 4 + ("f" * 6) + ("f" * 3) + ("H" * 4) * 2
size_payload = struct.calcsize(FORMAT_PAYLOAD)

port = '/dev/ttyACM0'
baud = 115200

app = Flask(__name__)
app.config['SECRET_KEY'] = ''
scheduler = APScheduler()
scheduler.init_app(app)
scheduler.start()
CORS(app)
socketio = SocketIO(app, cors_allowed_origins='*')

ser = serial.Serial(port, baud, timeout=10)

@app.route("/greet")
def greet():
  return """
    {
      "greeting": "Tommy"
    }
  """

@scheduler.task('interval', id='send_task', seconds=.3, misfire_grace_time=5)
def send_data():
  serial_sync = False
  data = b''
  while not serial_sync:
    try:
      data = ser.read_until(b'\xFF\xFF\xFF\xFF')
      serial_sync = True
    except Exception:
      pass 
  # print(data)

  data = data[:-4]
  if(len(data) == size_payload):
    # print(data)
    data_unpacked = unpack(FORMAT_PAYLOAD, data)
    print(data_unpacked)

    json_data = {
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
    }
    
    print(json_data)
    socketio.emit('data', json_data)

  # globals.lock.acquire(blocking=True)
  # if len(globals.data) != 12:
  #   return
  # print(globals.data)
  # json_data = {
  #   "temperature": {
  #     "motors": {
  #       "fl": globals.data[0],
  #       "fr": globals.data[1],
  #       "rl": globals.data[2],
  #       "rr": globals.data[3]
  #     }
  #   },
  #   "voltage": {
  #     "hv": {
  #       "high": globals.data[4],
  #       "low": globals.data[5],
  #       "avg": globals.data[6],
  #     },
  #     "lv": {
  #       "total": globals.data[7],
  #       "low": globals.data[8]
  #     },
  #   },
  #   "car": {
  #     "info": {
  #       "throttle": globals.data[9],
  #       "steeringangle": globals.data[10],
  #       "brake": globals.data[11]
  #     }
  #   }
  # }
  # globals.lock.release()
  # print(json_data)
  # socketio.emit('data', json_data)
  
read_thread = ReaderThread(ser, SerialReader)

if __name__ == '__main__':
  read_thread.start()
  socketio.run(app, host="0.0.0.0")
