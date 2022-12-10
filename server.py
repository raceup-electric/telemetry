from flask import Flask, send_from_directory
from flask_socketio import SocketIO, emit

from random import seed, random
import time

app = Flask(__name__, static_url_path='/', static_folder='dist')
socketio = SocketIO(app)
seed(852)


@app.route("/")
def hello_telemetry():
    return send_from_directory('dist', 'index.html')


def send():
    data = {
        "temperature": {
            "motors": {
                "fl": random(),
                "fr": random(),
                "rl": random(),
                "rr": random()
            },
            "igbt": {
                "fl": random(),
                "fr": random(),
                "rl": random(),
                "rr": random()
            },
            "inverter": {
                "fl": random(),
                "fr": random(),
                "rl": random(),
                "rr": random()
            },
            "cooling": {
                "preRad": random(),
                "preMot": random(),
                "preCold": random(),
                "postMot": random(),
                "postCold": random()
            },
            "hv": {
                "high": random(),
                "low": random(),
                "avg": random()
            },
            "lv": {
                "high": random()
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
            "velocity": {
                "avfl": random() * 250,
                "avfr": random() * 250,
                "avrl": random() * 250,
                "avrr": random() * 250,
                "av": random() * 250
            },
            "info": {
                "throttle": random(),
                "steeringangle": random(),
                "brake": random(),
                "brakepress": random()
            },
            "acceleration": {
                "x": random(),
                "y": random(),
                "z": random()
            },
            "omega": {
                "x": random(),
                "y": random(),
                "z": random()
            }
        }
    }
    emit('data', data)
