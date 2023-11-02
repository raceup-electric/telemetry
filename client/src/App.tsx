import { useState, useContext, SyntheticEvent } from 'react'

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
import { SB_CONTEXT } from './main';

interface Payload {
    payload: {
      new: { [key: string]: number };
  }
}

function App() {
  const supabase = useContext(SB_CONTEXT)!;

  // Page displayed
  const [page, setPage] = useState('dashboard');
  const handleChange = (_event: SyntheticEvent, newPage: string) => {
    setPage(newPage);
  };

  const [lastPayload, setNewPayload] = useState({new: {}});

  // Supabase event
  const ecu = supabase.channel('custom-insert-channel').on('postgres_changes', {
    event: 'INSERT',
    schema: 'public',
    table: import.meta.env.VITE_SB_TABLE,
  },
    (payload) => {
      setNewPayload(payload);
    }
  ).subscribe();

  return (
    <div className="App">
      <Box sx={{ width: '100%', typography: 'body1' }} />
      <TabContext value={page}>
        <Box sx={{ borderBottom: 1, borderColor: 'divider' }}>
          <TabList onChange={handleChange} aria-label="" variant="scrollable" orientation="vertical">
            <Tab label="Dashboard" value="dashboard" />
            {LOG_DEFS.map((def) => (
              <Tab key={def.id} label={def.label} value={def.id} />
            ))}
            <Tab label="Custom plot" value="customPlot" />
          </TabList>
        </Box>
        <TabPanel value="dashboard">
          <div className='innerBody'>
            <Dashboard payload={lastPayload} />
          </div>
        </TabPanel>
        {LOG_DEFS.map((def) => (
          <TabPanel key={def.id} value={def.id}>
            <div className='innerBody'>
              <DefaultPlots jRef={def.values} payload={lastPayload}></DefaultPlots>
            </div>
          </TabPanel>
        ))}
        <TabPanel value="customPlot">
          <div className='innerBody'>
            <CustomPlot payload={lastPayload} />
          </div>
        </TabPanel>
      </TabContext>
    </div>
  )
}

export default App
