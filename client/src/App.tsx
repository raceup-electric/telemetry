import React, { useContext, useEffect, useState } from 'react'
import { SocketContext } from './main';
import uPlot from 'uplot';
import UplotReact from 'uplot-react';
//import 'uplot/dist/uPlot.min.css';

import Button from '@mui/material/Button';
import Fab from '@mui/material/Fab';
import AddIcon from '@mui/icons-material/Add';
import TextField from '@mui/material/TextField';
import Select from '@mui/material/Select';
import Dialog from '@mui/material/Dialog';
import DialogActions from '@mui/material/DialogActions';
import DialogContent from '@mui/material/DialogContent';
import DialogContentText from '@mui/material/DialogContentText';
import DialogTitle from '@mui/material/DialogTitle';
import MenuItem from '@mui/material/MenuItem';

class Plot {
  container: Element;
  opts;
  jsonReference: String;
  uplot: uPlot;

  constructor(_container: Element, _opts, _jsonReference: String, _plot: uPlot) {
    this.container = _container;
    this.opts = _opts;
    this.jsonReference = _jsonReference;
    this.uplot = _plot;
  }
}

function App() {
  const [open, setOpen] = React.useState(false);
  const handleClickOpen = () => {
    setOpen(true);
  };
  const handleClose = () => {
    setOpen(false);
  };

  const plotter = [
    {
      value: 'temperature.motors.fl',
      label: 'Front left motor temperature',
    },
    {
      value: 'temperature.motors.rl',
      label: 'Right left motor temperature',
    }
  ];

  const [value, setValue] = React.useState('temperature.motors.fl');
  const handleChange = (event) => {
    setValue(event.target.value);
  };

  let plots = new Array<Plot>();;

  const socket = useContext(SocketContext)!;
  
  socket.on("connect", () => {
    console.log("Connected to SocketIO");
  })
  
  socket.on("data", (data) => {
    console.log(data);
    plots.forEach((plot) => {
      let value = ((plot.jsonReference).split(".")).reduce((a, c) => a[c], data);

      let data1 = plot.uplot.data;

      data1[0].push(data1[0].length);
      data1[1].push(value);

      plot.uplot.setData(data1);
    });
  });

  const handlePlot = () => {
    let newDiv = document.createElement('div');
    document.body.appendChild(newDiv);

    const opts = {
      title: value,
      width: 800,
      height: 500,
      series: [
        {
          label: "Date"
        },
        {
          label: "",
          points: { 
            show: true 
          },
          stroke: "blue"
        }
      ],
      scales: { x: { time: false } }
    };

    let newPlot = new uPlot(opts, [[], []], newDiv);
    plots.push(new Plot(newDiv, opts, value, newPlot));

    setOpen(false);
  };

  return (
    <div className="App">
      <button onClick={() => {
        socket.emit("hello", "server");
      }}>Click to emit</button>

      <Fab onClick={handleClickOpen} style={{
          'position': 'absolute', 
          'bottom': '5%', 
          'right': '3%'
        }} color="primary" aria-label="add">
        <AddIcon />
      </Fab>
      <Dialog open={open} onClose={handleClose}>
        <DialogTitle>Nuovo grafico</DialogTitle>
        <DialogContent> 
          <DialogContentText> Seleziona il dato che vuoi plottare </DialogContentText>
          <Select value={value} onChange={handleChange} defaultValue={'temperature.motors.fl'} id="reference" label="Option" fullWidth variant="standard">
            {plotter.map((option) => (
              <MenuItem key={option.value} value={option.value}>
                {option.label}
              </MenuItem>
            ))}
          </Select>
          <TextField label="Color Input" value={color} type={"color"} onChange={(e) => setColor(e.target.value)}/>
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
