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
    Chip,
    Tooltip,
    IconButton
} from "@mui/material/";
import AddIcon from '@mui/icons-material/Add';
import DownloadIcon from '@mui/icons-material/Download';

import { useState } from "react"

import { plotterOptions } from '../PlotterOptions';
import Grafico from "../components/Grafico";

function CustomPlot() {
    // Selected options
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

    // Plot storing
    const [grafici, setGrafici] = useState<JSX.Element[]>([]);
    const addPlot = (newPlot: JSX.Element) => {
        let temp: JSX.Element[] = [...grafici];
        temp.push(newPlot)
        setGrafici(temp)
    }

    // Append now plots on click
    const handlePlot = () => {
        if(value.length == 0) return;
        addPlot(<Grafico jsonReference={value} custom={true} _range={undefined} key={grafici.length} title="Custom" />);
        // Close dialog and reset selection
        setOpen(false);
        setValue([]);
    }

    return (
        <>
            {grafici}
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
        </>
    )
}

export default CustomPlot