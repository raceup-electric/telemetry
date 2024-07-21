from pathlib import Path
from asammdf import MDF
import tkinter as tk
from tkinter import filedialog

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

  for file in logsdir.iterdir():
    if file.match("*.mf4"):
      outfile = outdir.joinpath(file.stem + ".csv")
      try:
        mdf = MDF(file)
        print(f"Extracting file {file}")
        extracted = mdf.extract_bus_logging(databases)
        extracted.export("csv", outfile, single_time_base=True, time_as_date=True, empty_channels="zeros")
        print(f"Exported file {file} in {outfile}")
      except Exception as ex:
        print(f"Error while extracting messages: {ex}")
  
  print("Completed!")
