from flask import Flask
from flask_cors import CORS
from flask_socketio import SocketIO

app = Flask(__name__)
app.config['SECRET_KEY'] = ''
CORS(app)
socketio = SocketIO(app, cors_allowed_origins='*')

if __name__== '__main__':
  socketio.run(app)

@app.route("/greet")
def greet():
  return """
    {
      "greeting": "Tommy"
    }
  """

@socketio.on('hello')
def handle_message(data):
  print('received: ' + data)
  socketio.emit('hello', 'client')