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
    Stack
} from "@mui/material/";

import AddIcon from '@mui/icons-material/Add';

import { plotterOptions } from '../PlotterOptions';

import { useState } from "react"
import Box from "@mui/material/Box"
import Chip from "@mui/material/Chip"
import Plot, { MAX_POINT } from "../Plot"
import uPlot from "uplot"

function CustomPlot({ addPlot }: any) {
    const [value, setValue] = useState([]);
    const handleChange = (event: any) => {
        const { target: { value }, } = event;
        setValue(
            typeof value === 'string' ? value.split(',') : value,
        );
    };

    const [open, setOpen] = useState(false);
    const handleClickOpen = () => { setOpen(true); };
    const handleClose = () => { setOpen(false); };

    const handlePlot = () => {
        let newDiv = document.createElement('div');
        newDiv.className = "plot";
        document.body.getElementsByClassName('innerBody')[0].appendChild(newDiv);

        let series = [{
            label: "Time"
        },
        ];
        let data = [Array.from(Array(MAX_POINT).keys())];

        value.forEach(val => {
            data.push([]);
            series.push({
                label: val,
                paths: uPlot.paths.spline!(),
                points: {
                    show: true
                },
                stroke: "#" + String(Math.floor(Math.random() * 16777215).toString(16)), /*(u: uPlot, seriesIdx: number) => {
              let s = u.series[seriesIdx];
              let sc = u.scales[s.scale!];
    
              plotterOptions.forEach(option => {
                if(option.value === val)
                  return scaleGradient(u, s.scale!, 1, [[0, "green"],[option.redline, "red"],], true);
              },)
            }*/
            });
        });

        const opts = {
            title: value.toString(),
            width: newDiv.clientWidth,
            height: newDiv.clientHeight,
            pxAlign: 0,
            series: [...series],
            scales: {
                x: {
                    time: false
                }
            }
        };

        let newPlot = new uPlot(opts, data, newDiv);
        addPlot(newPlot);

        setOpen(false);
        setValue([]);
    };

    return (
        <>
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