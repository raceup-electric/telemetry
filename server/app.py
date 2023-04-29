from flask import Flask
from flask_cors import CORS
from flask_socketio import SocketIO

import time

from random import seed, random
seed(420)

app = Flask(__name__)
app.config['SECRET_KEY'] = ''
CORS(app)
socketio = SocketIO(app, cors_allowed_origins='*')

@app.route("/greet")
def greet():
  return """
    {
      "greeting": "Tommy"
    }
  """

@app.route("/data")
def send():
  # read data from SPI
  data = {
    "temperature": {
      "motors": {
        "fl": random(),
        "fr": random(),
        "rl": random(),
        "rr": random()
      }
    },
    "voltage": {
      "hv": {
        "high": random(),
        "low": random(),
        "avg": random(),
      },
      "lv": {
        "total": random(),
        "low": random()
      },
    },
    "car": {
      "info": {
        "throttle": random(),
        "steeringangle": random(),
        "brake": random()
      }
    }
  }
  socketio.emit('data', data)
  return data

# Da far diventare ogni 100ms
@socketio.on('hello')
def handle_message(data):
  send()

if __name__== '__main__':
  socketio.run(app)
