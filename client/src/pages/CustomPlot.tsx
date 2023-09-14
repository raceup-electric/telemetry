import {
    Button,
    Dialog,
    DialogActions,
    DialogContent,
    DialogContentText,
    DialogTitle,
    Fab,
    MenuItem,
    OutlinedInput,
    Select,
    Stack,
    Box,
    Chip
} from "@mui/material/";
import AddIcon from '@mui/icons-material/Add';

import { useContext, useEffect, useState } from "react"

import Grafico from "../components/Grafico";
import { SB_CONTEXT } from "../main";

interface Payload {
    payload: {
        new: { [key: string]: number };
    }
}

interface Plot {
    values: string[]
}

function CustomPlot( { payload }: Payload ) {
    const [columns, setColumns] = useState([] as JSX.Element[]);

    // Get columns from supabase
    const supabase = useContext(SB_CONTEXT)!;
    useEffect(() => {
        async function get_columns() {
            // Columns from test row
            let { data: result, error } = await supabase.from('ecu').select('*').eq('log_name', 'STEST0');
            let cols = Object.keys(result![0]).sort().filter(function(k) {
                // remove not plottable
                return !(k === "timestamp" || k === "log_name");
            }).map((option) => (
                // Map for dropdown
                <MenuItem key={option} value={option}>
                    {option}
                </MenuItem>
            ));
            setColumns(cols);
        };

        // trigger getter
        if (columns.length <= 0) {
            get_columns();
        }
    }, []);

    // Dropdown elected options
    const [value, setValue] = useState([]);
    const handleChange = (event: any) => {
        const { target: { value }, } = event;
        setValue(
            typeof value === 'string' ? value.split(',') : value,
        );
    };

    // Dialog status
    const [open, setOpen] = useState(false);
    const handleClickOpen = () => { setOpen(true); };
    const handleClose = () => { setOpen(false); };

    
    const [grafici, setGrafici] = useState([] as Plot[]);
    const handlePlot = () => {
        if (value.length == 0) return;

        let newGrafici = [...grafici, {"values": value.sort()}];

        setGrafici(newGrafici);
        setOpen(false);
        setValue([]);
    }

    return (
        <>
            {grafici.map((grafico) => (
                <Grafico jsonReference={grafico.values} payload={payload} key={grafici.length} title="Custom" />
            ))}
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
                            {columns}
                        </Select>
                    </Stack>
                </DialogContent>
                <DialogActions>
                    <Button onClick={handleClose}>Cancel</Button>
                    <Button onClick={handlePlot}>Plot</Button>
                </DialogActions>
            </Dialog>
        </>
    )
}

export default CustomPlot