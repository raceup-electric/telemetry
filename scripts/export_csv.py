from pathlib import Path
from tempfile import NamedTemporaryFile
from asammdf import MDF
import tkinter as tk
from tkinter import filedialog
import pandas as pd
import csv

RESAMPLE_PERIOD = "5ms"

if __name__ == "__main__":
    root = tk.Tk()
    root.withdraw() 

    logsdir = Path(filedialog.askdirectory(title="Select logs folder"))
    outdir = logsdir / "csv"
    outdir.mkdir(exist_ok=True)

    dbcfiles = filedialog.askopenfilenames(
        title="Select can1.dbc and can2.dbc",
        filetypes=(("DBC files", "*.dbc"), ("All files", "*.*"))
    )
    databases = {"CAN": []}
    for dbcfile in dbcfiles:
        path = Path(dbcfile)
        if path.stem == "can1":
            databases["CAN"].append((path.resolve(), 1))
        elif path.stem == "can2":
            tmp = NamedTemporaryFile(mode="w", suffix=".dbc", delete=False)
            with open(path, "r") as fin, tmp:
                for line in fin:
                    if not line.lstrip().startswith("SG_MUL_VAL_"):
                        tmp.write(line)
            databases["CAN"].append((Path(tmp.name), 2))

    if len(databases["CAN"]) != 2:
        print("You need to provide both can1.dbc and can2.dbc")
        exit(1)

    for file in logsdir.glob("*.mf4"):
        outfile = outdir / (file.stem + ".csv")
        try:
            mdf = MDF(file)
            print(f"Extracting file {file}")
            extracted = mdf.extract_bus_logging(databases)
            df = extracted.to_dataframe(
                time_as_date=True,
                use_display_names=True
            )
            df.index = df.index.tz_convert("Europe/Rome")
            df = df.apply(pd.to_numeric, errors="coerce")
            df = df.resample(RESAMPLE_PERIOD).mean()

            print(f"Exporting file {file} to CSV")
            df.to_csv(
                outfile,
                index=True,
                index_label="timestamps",
                quoting=csv.QUOTE_NONNUMERIC
            )
            print(f"Exported file {file} in {outfile}\n")

        except KeyboardInterrupt:
            print("User interrupted the process")
            break
        except Exception as ex:
            print(f"Error while extracting log {file}. Error: {ex}")

    print("Completed!")
