from serial.threaded import LineReader
import globals

class SerialReader(LineReader):
  def handle_line(self, line: str) -> None:
    globals.lock.acquire()
    globals.data = []
    try:
      for elt in list(map(int, line.split(','))):
        globals.data.append(elt)
    except:
      pass
    finally:
      globals.lock.release()
