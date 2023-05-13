import { useContext, useState, SyntheticEvent } from 'react'
import { SocketContext } from './main';

import Plot from './Plot';
import ConnectionAlert from './components/ConnectionAlert';
import CustomPlot from './pages/CustomPlot';

import { 
  Tab,
  Box,
} from "@mui/material/";

import {
  TabContext,
  TabList,
  TabPanel
} from '@mui/lab/';

import PreloadedPlots from './pages/PreloadedPlots';

function App() {
  const socket = useContext(SocketContext)!;

  window.addEventListener("resize", () => {
    // TODO: controllare funzionamento
    plots.forEach((plot) => {
      plot.uplot.setSize({width: plot.container.clientWidth, height: plot.container.clientHeight});
    });
  });

  const [connected, setConnected] = useState(false);

  const [plots, addPlot] = useState<Plot[]>([]);
  const addPlotToArray = (p: Plot) => {
    addPlot([...plots, p]);
  };

  const [page, setPage] = useState('temperature.motors.');
  const handleChange = (_event: SyntheticEvent, newPage: string) => {
    setPage(newPage);
  };

  socket.on("connect", () => {
    setConnected(true);
  });

  socket.on('connect_error', function() {
    setConnected(false);
  });
  
  socket.on("data", (data) => {
    plots.forEach((plot) => {
      plot.appendPoint(data);
    });
  });

  return (
    <div className="App">
      {connected ? <></> : <ConnectionAlert />}
      <Box sx={{ width: '100%', typography: 'body1' }} />
      <TabContext value={page}>
        <Box sx={{ borderBottom: 1, borderColor: 'divider' }}>
          <TabList onChange={handleChange} aria-label="">
            <Tab label="Temperatura motori" value="temperature.motors." />
            <Tab label="High Voltage" value="voltage.hv." />
            <Tab label="Low Voltage" value="voltage.lv." />
            <Tab label="Car info" value="car.info." />
            <Tab label="Custom plot" value="customPlot" />
          </TabList>
        </Box>
        <TabPanel value="temperature.motors.">
          <PreloadedPlots page="temperature.motors." title="Temperatura motori" addPlot={addPlotToArray}></PreloadedPlots>
        </TabPanel>
        <TabPanel value="voltage.hv.">
          <PreloadedPlots page="voltage.hv." title="High Voltage" addPlot={addPlotToArray}></PreloadedPlots>
        </TabPanel>
        <TabPanel value="voltage.lv.">
          <PreloadedPlots page="voltage.lv." title="Low Voltage" addPlot={addPlotToArray}></PreloadedPlots>
        </TabPanel>
        <TabPanel value="car.info.">
          <PreloadedPlots page="car.info." title="Car info" addPlot={addPlotToArray}></PreloadedPlots>
        </TabPanel>
        <TabPanel value="customPlot">
          <div className='innerBody'>
            <CustomPlot addPlot={addPlotToArray} />
          </div>
        </TabPanel>
      </TabContext>
    </div>
  )
}

export default App
