import { useState, useContext, SyntheticEvent, createElement } from 'react'

import {
    Tab,
    Box,
    Tabs,
    Avatar,
    Stack,
    Grid,
} from "@mui/material/";

import {
    TabContext,
    TabList,
    TabPanel
} from '@mui/lab/';

import CustomPlot from './pages/CustomPlot';
import DashBoard from './pages/DashBoard';

import {
    AutoFixHigh,
    Dashboard
} from '@mui/icons-material';

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

    const [lastPayload, setNewPayload] = useState({ new: {} });

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
            <Stack direction="row" spacing={0} useFlexGap flexWrap="wrap" sx={{ height: '100%' }}>
                <Box
                    sx={{
                        flexGrow: 1,
                        bgcolor: 'background.paper',
                        display: 'flex',
                        position: 'fixed',
                        width: '17.5vw',
                        height: '100%',
                        borderRight: 1,
                        borderColor: 'divider'
                    }}>
                    <Stack spacing={5}>
                        <img
                            src={'../logo.png'}
                            loading="lazy"
                            style={{
                                position: 'relative',
                                top: '2%',
                                width: '35%',
                                left: '50%',
                                transform: 'translateX(-50%)'
                            }}
                        />
                        <Tabs
                            orientation="vertical"
                            variant="scrollable"
                            value={page}
                            onChange={handleChange}
                            sx={{
                                position: 'relative',
                                height: '80%'
                            }}
                        >
                            <Tab icon={<Dashboard />} iconPosition="start" label="Dashboard" value="dashboard" />
                            {LOG_DEFS.map((def) => (
                                <Tab key={def.id} icon={createElement(def.icon)} iconPosition="start" label={def.label} value={def.id} />
                            ))}
                            <Tab icon={<AutoFixHigh />} iconPosition="start" label="Custom plot" value="customPlot" />
                        </Tabs>
                    </Stack>
                </Box>
                <Box sx={{
                    flexGrow: 1,
                    bgcolor: 'background.paper',
                    display: 'flex',
                    marginLeft: '18vw',
                    position: 'fixed',
                    height: '100%'
                }}>
                    <TabContext value={page}>
                        <TabPanel value="dashboard" sx={{
                            height: '100%',
                            overflow: 'auto'
                        }}>
                            <div style={{ marginTop: '5%' }}>
                                <DashBoard payload={lastPayload} />
                            </div>
                        </TabPanel>
                        {LOG_DEFS.map((def) => (
                            <TabPanel key={def.id} value={def.id} sx={{
                                width: '100vw',
                                height: '100%',
                                overflow: 'auto'
                            }}>
                                <div style={{ marginTop: '5%' }}>
                                    <DefaultPlots jRef={def.values} payload={lastPayload}></DefaultPlots>
                                </div>
                            </TabPanel>
                        ))}
                        <TabPanel value="customPlot" sx={{
                            height: '100%',
                            overflow: 'auto'
                        }}>
                            <div style={{ marginTop: '5%' }}>
                                <CustomPlot payload={lastPayload} />
                            </div>
                        </TabPanel>
                    </TabContext>
                </Box>
            </Stack>
        </div>
    )
}

export default App
