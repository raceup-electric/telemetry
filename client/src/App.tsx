import { useContext, useState } from 'react'
import { SocketContext } from './main';
import uPlot from 'uplot';

import { Button, Fab, 
  TextField, Select, 
  Dialog, DialogActions, DialogContent, DialogContentText, DialogTitle, MenuItem, 
  Stack
} from '@mui/material'

import AddIcon from '@mui/icons-material/Add';

import { plotterOptions } from './PlotterOptions';
import scaleGradient from './utils';

const MAX_POINT = 60;

class Plot {
  container: Element;
  opts;
  jsonReference: String;
  uplot: uPlot;
  pointsPlotted: number = 0;

  constructor(_container: Element, _opts: object, _jsonReference: String, _plot: uPlot) {
    this.container = _container;
    this.opts = _opts;
    this.jsonReference = _jsonReference;
    this.uplot = _plot;
  }

  getPointsPlotted() {
    return this.pointsPlotted;
  }

  addPoint() {
    this.pointsPlotted = this.pointsPlotted + 1;
  }
}

function App() {
  window.addEventListener("resize", e => {
    plots.forEach((plot) => {
      plot.uplot.setSize({width: plot.container.clientWidth, height: plot.container.clientHeight});
    });
  });

  const socket = useContext(SocketContext)!;
  
  const [open, setOpen] = useState(false);
  const handleClickOpen = () => { setOpen(true); };
  const handleClose = () => { setOpen(false); };

  const [value, setValue] = useState('temperature.motors.fl');
  
  let plots = new Array<Plot>();

  socket.on("connect", () => {
    console.log("Connected to SocketIO");
  })
  
  socket.on("data", (data) => {
    plots.forEach((plot) => {
      let value = ((plot.jsonReference).split(".")).reduce((a, c) => a[c], data);

      let data1 = plot.uplot.data;
      console.log(data1[0])

      //data1[0].push(plot.getPointsPlotted());
      data1[1].push(value);
      //data1[0].push(plot.getPointsPlotted() + 1);

      if(plot.getPointsPlotted() >= MAX_POINT){
        data1[0].push(plot.getPointsPlotted() + 1);
        data1[1].shift();
        data1[0].shift();
      }

      plot.uplot.setData(data1);
      plot.addPoint();
    });
  });

  const handlePlot = () => {
    let newDiv = document.createElement('div');
    newDiv.className = "plot";
    document.body.appendChild(newDiv);

    let title = "";
    let redLine: number = 10000;
    plotterOptions.forEach(option => {
      if(option.value === value){
        title = option.label;
        redLine = option.redline
      }
    });

    const opts = {
      title: title,
      width: newDiv.clientWidth,
      height: newDiv.clientHeight,
      pxAlign: 0,
      series: [{
          label: "Date"
        },
        {
          label: title,
          paths: uPlot.paths.spline(),
          points: { 
            show: true 
          },
          stroke: (u: uPlot, seriesIdx: number) => {
            let s = u.series[seriesIdx];
            let sc = u.scales[s.scale];

            return scaleGradient(u, s.scale, 1, [[0, "green"],[redLine, "red"],], 1);
          }
        }
      ],
      scales: {
        x: {
          time: false
        }
      }
    };

    let newPlot = new uPlot(opts, [Array.from(Array(MAX_POINT).keys()), []], newDiv);
    plots.push(new Plot(newDiv, opts, value, newPlot));

    setOpen(false);
  };

  return (
    <div className="App">
      <button style={{position: 'absolute'}} onClick={() => {
        socket.emit("hello", "server");
      }}>Click to emit</button>
      <Fab onClick={handleClickOpen} style={{
          'position': 'fixed', 
          'bottom': '5%', 
          'right': '3%'
        }} color="primary" aria-label="add">
        <AddIcon />
      </Fab>
      <Dialog open={open} onClose={handleClose}>
        <DialogTitle>Nuovo grafico</DialogTitle>
        <DialogContent> 
          <Stack spacing={2}>
            <DialogContentText> Seleziona il dato che vuoi plottare </DialogContentText>
            <Select value={value} onChange={(e) => setValue(e.target.value)} defaultValue={'temperature.motors.fl'} id="reference" label="Option" fullWidth>
              {plotterOptions.map((option) => (
                <MenuItem key={option.value} value={option.value}>
                  {option.label}
                </MenuItem>
              ))}
            </Select>
          </Stack>
        </DialogContent>
        <DialogActions>
          <Button onClick={handleClose}>Cancel</Button>
          <Button onClick={handlePlot}>Plot</Button>
        </DialogActions>
      </Dialog>
    </div>
  )
}

export default App
