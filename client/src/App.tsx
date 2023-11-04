import { useState, useContext, SyntheticEvent, createElement } from 'react'

import {
    Tab,
    Box,
    Tabs,
    Stack
} from "@mui/material/";

import {
    TabContext,
    TabPanel
} from '@mui/lab/';

import CustomPlot from './pages/CustomPlot';
import DashBoard from './pages/DashBoard';

import {
    AutoFixHigh,
    BatteryCharging90,
    Dashboard
} from '@mui/icons-material';

import { LOG_DEFS } from './log_defs';
import DefaultPlots from './pages/DefaultPlots';
import { SB_CONTEXT } from './main';
import Bms2 from './pages/Bms';

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

    const [ecuPayload, setEcuPayload] = useState({ new: {} });
    const [bmshvPayload, setBmsHvPayload] = useState({ new: {} });

    // Supabase event
    const ecu = supabase.channel('custom-insert-channel').on('postgres_changes', {
        event: 'INSERT',
        schema: 'public',
        table: import.meta.env.VITE_SB_ECU_TABLE,
    },
        (payload) => {
            setEcuPayload(payload);
        }
    ).subscribe();

    const bmshv = supabase.channel('custom-insert-channel').on('postgres_changes', {
        event: 'INSERT',
        schema: 'public',
        table: import.meta.env.VITE_SB_BMSHV_TABLE,
    },
        (payload) => {
            setBmsHvPayload(payload);
            console.log(payload);
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
                            <Tab icon={<BatteryCharging90 />} iconPosition="start" label="Bms HV" value="bmshv" />
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
                                <DashBoard payload={ecuPayload} />
                            </div>
                        </TabPanel>
                        <TabPanel value="bmshv" sx={{
                            width: '79vw',
                            height: '100%',
                            overflow: 'auto'
                        }}>
                            <div style={{ marginTop: '0%', marginBottom: '4%', width: '100%' }}>
                                <Bms2 payload={bmshvPayload} />
                            </div>
                        </TabPanel>
                        {LOG_DEFS.map((def) => (
                            <TabPanel key={def.id} value={def.id} sx={{
                                width: '100vw',
                                height: '100%',
                                overflow: 'auto'
                            }}>
                                <div style={{ marginTop: '5%' }}>
                                    <DefaultPlots jRef={def.values} payload={ecuPayload}></DefaultPlots>
                                </div>
                            </TabPanel>
                        ))}
                        <TabPanel value="customPlot" sx={{
                            width: '100vw',
                            height: '100%',
                            overflow: 'auto'
                        }}>
                            <div style={{ marginTop: '5%' }}>
                                <CustomPlot payload={ecuPayload} />
                            </div>
                        </TabPanel>
                    </TabContext>
                </Box>
            </Stack>
        </div>
    )
}

export default App
