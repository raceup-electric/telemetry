from serial.threaded import LineReader
import globals
import numpy

class SerialReader(LineReader):
  def handle_line(self, line: str) -> None:
    globals.lock.acquire()
    globals.data = []
    for elt in numpy.fromstring(line, dtype=int, sep=','):
      globals.data.append(elt)
    globals.lock.release()