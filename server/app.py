from flask import Flask
from flask_cors import CORS
from flask_socketio import SocketIO
from reader import SerialReader
from flask_apscheduler import APScheduler

port = '/dev/tty.usbmodem101'
baud = 9600

app = Flask(__name__)
app.config['SECRET_KEY'] = ''
scheduler = APScheduler()
scheduler.init_app(app)
scheduler.start()
CORS(app)
socketio = SocketIO(app, cors_allowed_origins='*')

reader = SerialReader(port, baud)

@app.route("/greet")
def greet():
  return """
    {
      "greeting": "Tommy"
    }
  """

def send_data():
  # read data from serial
  val = reader.read()
  data = {
    "temperature": {
      "motors": {
        "fl": val[0],
        "fr": val[1],
        "rl": val[2],
        "rr": val[3]
      }
    },
    "voltage": {
      "hv": {
        "high": val[4],
        "low": val[5],
        "avg": val[6],
      },
      "lv": {
        "total": val[7],
        "low": val[8]
      },
    },
    "car": {
      "info": {
        "throttle": val[9],
        "steeringangle": val[10],
        "brake": val[11]
      }
    }
  }
  socketio.emit('data', data)

scheduler.add_job(id='send_task', func=send_data, trigger='interval', seconds=0.5)

if __name__== '__main__':
  socketio.run(app)