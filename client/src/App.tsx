import { useContext, useEffect, useState } from 'react'
import { SocketContext } from './main';
import uPlot from 'uplot';

import { Button, Fab, 
  Select, 
  Dialog, DialogActions, DialogContent, DialogContentText, DialogTitle, MenuItem, 
  Stack,
  Box,
  OutlinedInput,
  Chip,
  Alert,
  AlertTitle,
  createTheme,
  ThemeProvider,
  CssBaseline
} from '@mui/material'

import AddIcon from '@mui/icons-material/Add';

import { plotterOptions } from './PlotterOptions';
import scaleGradient from './utils';
import Plot from './Plot';

const MAX_POINT = 60;
const darkTheme = createTheme({
  palette: {
    mode: 'dark',
  },
});

function App() {
  window.addEventListener("resize", e => {
    plots.forEach((plot) => {
      plot.uplot.setSize({width: plot.container.clientWidth, height: plot.container.clientHeight});
    });
  });

  let alert = <div className="alert">
    <Alert severity="error">
      <AlertTitle>Server error</AlertTitle>
        Connection error — <strong>check the RPi</strong>
    </Alert>
  </div>;

  const socket = useContext(SocketContext)!;
  const [connected, setConnected] = useState(false);
  
  const [open, setOpen] = useState(false);
  const handleClickOpen = () => { setOpen(true); };
  const handleClose = () => { setOpen(false); };

  const [value, setValue] = useState([]);
  const handleChange = (event) => {
    const { target: { value }, } = event;
    setValue(
      typeof value === 'string' ? value.split(',') : value,
    );
  };
  
  let plots = new Array<Plot>();

  socket.on("connect", () => {
    setConnected(true);
    alert = "";
  });

  socket.on('connect_error', function(err) {
    setConnected(false);
  });
  
  socket.on("data", (data) => {
    console.log(data);
    plots.forEach((plot) => {
      let data1 = plot.uplot.data;    
      plot.jsonReference.forEach((jsonRef, i) => {
        let value = ((jsonRef).split(".")).reduce((a, c) => a[c], data);
  
        console.log(data1);
        
        data1[i+1].push(value);

        if(plot.getPointsPlotted() >= MAX_POINT){
          if(i == 0) data1[0].push(plot.getPointsPlotted() + 1);
        }
      });
      if(plot.getPointsPlotted() >= MAX_POINT){
        data1.forEach((dat) => {
          dat.shift();
        })
      }
      plot.uplot.setData(data1);
      plot.addPoint();
    });
  });

  const handlePlot = () => {
    let newDiv = document.createElement('div');
    newDiv.className = "plot";
    document.body.appendChild(newDiv);

    let series = [{
        label: "Time"
      },
    ];
    let data = [Array.from(Array(MAX_POINT).keys())];

    value.forEach(val => {
      data.push([]);
      series.push({
        label: val,
        paths: uPlot.paths.spline!(),
        points: { 
          show: true 
        },
        stroke: "#" + String(Math.floor(Math.random()*16777215).toString(16)), /*(u: uPlot, seriesIdx: number) => {
          let s = u.series[seriesIdx];
          let sc = u.scales[s.scale!];

          plotterOptions.forEach(option => {
            if(option.value === val)
              return scaleGradient(u, s.scale!, 1, [[0, "green"],[option.redline, "red"],], true);
          },)
        }*/
      });
    })

    const opts = {
      title: value.toString(),
      width: newDiv.clientWidth,
      height: newDiv.clientHeight,
      pxAlign: 0,
      series: [...series],
      scales: {
        x: {
          time: false
        }
      }
    };

    let newPlot = new uPlot(opts, data, newDiv);
    plots.push(new Plot(newDiv, opts, value, newPlot));

    setOpen(false);
    setValue([]);
  };

  return (
    //<ThemeProvider theme={darkTheme}>
    //<CssBaseline />
    <div className="App">
      {
      /*<button style={{position: 'absolute'}} onClick={() => {
        socket.emit("hello", "server");
      }}>Click to emit</button>*/
      }
      {connected ? <></> : alert}
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
            <Select id="reference" label="Option" fullWidth multiple
              value={value}
              onChange={handleChange}
              input={<OutlinedInput id="select-multiple-chip" label="Chip" />}
              renderValue={(selected) => (
                <Box sx={{ display: 'flex', flexWrap: 'wrap', gap: 0.5 }}>
                  {selected.map((value) => (
                    <Chip key={value} label={value} />
                  ))}
                </Box>
              )}>
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
    //</ThemeProvider>
  )
}

export default App
