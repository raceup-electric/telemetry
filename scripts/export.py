from pathlib import Path
from asammdf import MDF
import tkinter as tk
from tkinter import filedialog
from tqdm import tqdm

def getprogcb():
  def progcb(current, max):
    pbar.n = current
    pbar.total = max
    pbar.refresh()
  progcb.stop = False

  return progcb

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
        print(f"\nExtracting file {file}")
        with tqdm(total=100, desc="File extraction", dynamic_ncols=True) as pbar:
          extracted = mdf.extract_bus_logging(databases, progress=getprogcb())
        print(f"\nExporting file {file} to CSV")
        with tqdm(total=100, desc="CSV export", dynamic_ncols=True) as pbar:
          extracted.export("csv", outfile, single_time_base=True, time_as_date=True, empty_channels="zeros", progress=getprogcb())
        print(f"\nExported file {file} in {outfile}\n")
      except KeyboardInterrupt:
        print("User interrupted the process")
      except Exception as ex:
        print(f"Error while extracting log {file}. Error: {ex}")
  
  print("Completed!")
