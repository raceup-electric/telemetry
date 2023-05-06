import serial

class Reader:
  def read() -> dict:
    pass

class SerialReader(Reader):

  ser = serial.Serial()

  def __init__(self, port: str, baud: int) -> None:
    self.ser.port = port
    self.ser.baudrate = baud
    self.ser.open()

  def __del__(self):
    self.ser.close()

  def read(self):
    data = self.ser.read(24)
    val = []
    for _ in range(0, 12):
      # val.append((int)(data[0:2]))
      val.append(((int)(data[1]) << 8) + (int)(data[0]))
      data = data[2:]
    return val