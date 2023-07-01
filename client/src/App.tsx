import { useContext, useState, SyntheticEvent } from 'react'
import { SocketContext } from './main';

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
import { optionsGroup } from './PlotterOptions';

function App() {
  const socket = useContext(SocketContext)!;

  const [connected, setConnected] = useState(false);

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

  return (
    <div className="App">
      {connected ? <></> : <ConnectionAlert />}
      <Box sx={{ width: '100%', typography: 'body1' }} />
      <TabContext value={page}>
        <Box sx={{ borderBottom: 1, borderColor: 'divider' }}>
          <TabList onChange={handleChange} aria-label="" variant="scrollable">
            {optionsGroup.map(opt => <Tab label={opt.label} value={opt.id} key={opt.id} />)}
            <Tab label="Custom plot" value="customPlot" />
          </TabList>
        </Box>
        {optionsGroup.map(opt => (
          <TabPanel value={opt.id} key={opt.id}>
            <PreloadedPlots jRef={opt.values} page={opt.id} title={opt.label} ></PreloadedPlots>
          </TabPanel>
        ))}
        <TabPanel value="customPlot">
          <div className='innerBody'>
            <CustomPlot />
          </div>
        </TabPanel>
      </TabContext>
    </div>
  )
}

export default App
