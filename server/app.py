import serial
from serial.threaded import ReaderThread
from flask import Flask
from flask_apscheduler import APScheduler
from flask_cors import CORS
from flask_socketio import SocketIO
from reader import SerialReader
import time
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

@scheduler.task('interval', id='send_task', seconds=.2, misfire_grace_time=5)
def send_data():    
    # print(globals.data)
  globals.lock.acquire()
  if (time.time() - globals.last_received < 2):
    socketio.emit('data', globals.data)
  
  globals.lock.release()
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
