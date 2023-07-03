import { useContext, useState, SyntheticEvent } from 'react'
import { SocketContext } from './main';

import ConnectionAlert from './components/ConnectionAlert';
import CustomPlot from './pages/CustomPlot';

import SignalCellularAltIcon from '@mui/icons-material/SignalCellularAlt';
import NetworkCheckIcon from '@mui/icons-material/NetworkCheck';

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
  // Socket
  const socket = useContext(SocketContext)!;

  // Connection state
  const [connected, setConnected] = useState(false);
  socket.on("connect", () => {
    setConnected(true);
  });
  socket.on('connect_error', function() {
    setConnected(false);
  });

  // Page displayed
  const [page, setPage] = useState('HV');
  const handleChange = (_event: SyntheticEvent, newPage: string) => {
    setPage(newPage);
  };

  return (
    <div className="App">
      {connected ? <></> : <ConnectionAlert />}
      <SignalCellularAltIcon id="rssi" fontSize="large" style={{
        'position': 'fixed', 
        'top': '10%', 
        'right': '3%'
      }} />
      <NetworkCheckIcon id="snr" fontSize="large" style={{
        'position': 'fixed', 
        'top': '17%', 
        'right': '3%'
      }} />
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
            <PreloadedPlots jRef={opt.values} ></PreloadedPlots>
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
