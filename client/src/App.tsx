import { useState, SyntheticEvent } from 'react'

import { 
  Tab,
  Box,
} from "@mui/material/";

import {
  TabContext,
  TabList,
  TabPanel
} from '@mui/lab/';

import CustomPlot from './pages/CustomPlot';
import Dashboard from './pages/DashBoard';

import { LOG_DEFS } from './log_defs';
import DefaultPlots from './pages/DefaultPlots';

function App() {

  // Page displayed
  const [page, setPage] = useState('dashboard');
  const handleChange = (_event: SyntheticEvent, newPage: string) => {
    setPage(newPage);
  };

  return (
    <div className="App">
      <Box sx={{ width: '100%', typography: 'body1' }} />
      <TabContext value={page}>
        <Box sx={{ borderBottom: 1, borderColor: 'divider' }}>
          <TabList onChange={handleChange} aria-label="" variant="scrollable">
            <Tab label="Dashboard" value="dashboard" />
            {LOG_DEFS.map((def) => (
              <Tab key={def.id} label={def.label} value={def.id} />
            ))}
            <Tab label="Custom plot" value="customPlot" />
          </TabList>
        </Box>
        <TabPanel value="dashboard">
          <div className='innerBody'>
            <Dashboard />
          </div>
        </TabPanel>
        {LOG_DEFS.map((def) => (
          <TabPanel key={def.id} value={def.id}>
            <div className='innerBody'>
              <DefaultPlots jRef={def.values}></DefaultPlots>
            </div>
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
