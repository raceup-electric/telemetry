import serial
from serial.threaded import ReaderThread
from flask import Flask
from flask_apscheduler import APScheduler
from flask_cors import CORS
from flask_socketio import SocketIO
from reader import SerialReader
import time
import globals

import random

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

# Default Arduino serial port on RPi
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

@scheduler.task('interval', id='send_task', seconds=.2, misfire_grace_time=5)
def send_data():
  globals.lock.acquire()
  globals.data["timestamp"] = time.time() - globals.start_time
  if (time.time() - globals.last_received < 2) and not (globals.lora_error):
    socketio.emit('data', globals.data)
  # else:
  #   socketio.emit('lora_error')
  
  globals.lock.release()
  
read_thread = ReaderThread(ser, SerialReader)

if __name__ == '__main__':
  read_thread.start()
  socketio.run(app, host="0.0.0.0")
