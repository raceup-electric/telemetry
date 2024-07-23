from pathlib import Path
from asammdf import MDF
import tkinter as tk
from tkinter import filedialog

from influxdb_client import InfluxDBClient, WriteOptions, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS

import numpy as np

RESAMPLE_PERIOD = "5ms"

INFLUXDB_URL = ""
INFLUXDB_TOKEN = ""
INFLUXDB_ORG = ""
INFLUXDB_BUCKET = ""

if __name__ == "__main__":
  root = tk.Tk()
  root.withdraw()  # Hide the main window

  logsdir = filedialog.askdirectory(
    title="Select logs folder"
  )
  logsdir = Path(logsdir)
  outdir = logsdir.joinpath("csv")
  outdir.mkdir(exist_ok=True)

  databases = {
    "CAN": []
  }
  dbcfiles = filedialog.askopenfilenames(
      title="Select database files",
      filetypes=(("DBC files", "*.dbc"), ("All files", "*.*"))
  )
  for dbcfile in dbcfiles:
    path = Path(dbcfile)
    if path.stem == "can1":
      databases["CAN"].append((path.resolve(), 1))
    if path.stem == "can2":
      databases["CAN"].append((path.resolve(), 2))
  if len(databases["CAN"]) != 2:
    print("You need to provide can1.dbc and can2.dbc")
    exit(1)

  root.destroy()

  with InfluxDBClient(url=INFLUXDB_URL, token=INFLUXDB_TOKEN, org=INFLUXDB_ORG) as client:
    options = WriteOptions(write_type=SYNCHRONOUS)
    with client.write_api(write_options=options) as writer:
      for file in logsdir.glob("*.mf4"):
        try:
          mdf = MDF(file)

          extracted = mdf.extract_bus_logging(database_files=databases)
          df = extracted.to_dataframe(time_as_date=True, use_display_names=True)
          df.index = df.index.tz_convert("Europe/Rome")
          df = df.resample(RESAMPLE_PERIOD).mean()

          print(f"Exporting log {file}")
          
          for index, row in df.iterrows():
            point = Point("car").time(index, WritePrecision.MS)
            for col, value in row.items():
              if isinstance(value, (int, float, np.number)) and not np.isnan(value):
                point.field(col, value)
            writer.write(bucket=INFLUXDB_BUCKET, org=INFLUXDB_ORG, record=point, write_precision=WritePrecision.MS) 

          print(f"Log {file} exported to DB successfully!")

        except KeyboardInterrupt:
          print("User interrupted the process")
        except Exception as ex:
          print(f"Error while extracting log {file}. Error: {ex}")
      
      print("Completed!")