from threading import Lock
import time

start_time = time.time()
data = {
    "timestamp": time.time() - start_time,
    "RSSI": -500,
    "SNR": -500,
    "car_status": 0,
    "temperature": {
      "motors": {
        "fl": 0,
        "fr": 0,
        "rl": 0,
        "rr": 0,
      },
      "IGBT": {
        "fl": 0,
        "fr": 0,
        "rl": 0,
        "rr": 0,
      },
      "inverter": {
        "fl": 0,
        "fr": 0,
        "rl": 0,
        "rr": 0
      }
    },
    "status": {
      "motor": {
        "fl": 0,
        "fr": 0,
        "rl": 0,
        "rr": 0
      }
    },
    "voltage": {
      "hv": {
        "max": 3000,
        "min": 3000,
        "mean": 3000
        },
        "lv": {
          "min": 3000,
          "tot": 3000
        }
    },
    "temp": {
      "hv": {
        "max": 0,
        "min": 0,
        "mean": 0
      },
      "lv": {
        "max": 0
      },
      "dx": {
        "precold": 0,
        "postcold": 0,
        "premot": 0,
        "postmot": 0,
      },
      "sx": {
        "precold": 0,
        "postcold": 0,
        "premot": 0,
        "postmot": 0,
      }
    },
    "ERROR_PKT": 0,
    "TOTAL_PKT": 0,
}
lock = Lock()
last_received = time.time()
lora_error = False
reader_error = False
