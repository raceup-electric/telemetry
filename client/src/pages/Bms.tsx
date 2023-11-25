import { Paper, Table, TableBody, TableCell, TableContainer, TableHead, TableRow } from "@mui/material"
import { useEffect, useState } from "react";

interface PayLoad {
    payload: {
        new: { [key: string]: number };
    }
}

const SEGMENTS = 8;
const CELLS = 16;
const TEMPS = 3;

let dataMatrix: number[][] = new Array(CELLS + TEMPS).fill(0).map(() => new Array(SEGMENTS).fill(0));

function Bms({ payload }: PayLoad) {
    const [width, setWidth] = useState(document.body.clientWidth);
    const [height, setHeight] = useState(document.body.clientHeight);
    const setWindowDimensions = () => {
        setWidth(document.body.clientWidth);
        setHeight(document.body.clientHeight);
    }

    const [newPayload, setNewPayload] = useState(payload);

    useEffect(() => {
        window.addEventListener('resize', setWindowDimensions);
        return () => {
            window.removeEventListener('resize', setWindowDimensions)
        }
    }, []);

    useEffect(() => {
        setNewPayload(payload);
        for(let i = 0; i < CELLS + TEMPS; i++) {
            for(let j = 0; j < SEGMENTS; j++) {
                dataMatrix[i][j] = i < CELLS ? payload.new["cell_" + j + "_" + i] : payload.new["temp_" + j + "_" + (i-CELLS+1)];
                if(dataMatrix[i][j] === null) dataMatrix[i][j] = 0;
            }
        }
    }, [payload])

    return (
        <>
            <TableContainer component={Paper}>
                <Table sx={{ position: 'relative' }} size="small" aria-label="simple table">
                    <TableHead>
                        <TableRow>
                            {Array.from(Array(SEGMENTS).keys()).map((seg) => (
                                <TableCell align="center" key={"header" + seg}><b>Seg. {seg}</b></TableCell>
                            ))}
                        </TableRow>
                    </TableHead>
                    <TableBody>
                        <>
                            {Array.from(Array(CELLS).keys()).map((cell) => (
                                <TableRow key={cell}>
                                    {Array.from(Array(SEGMENTS).keys()).map((seg) => (
                                        <TableCell 
                                            sx={{
                                                backgroundColor: 
                                                    (dataMatrix[cell][seg] >= 3.5 && dataMatrix[cell][seg] <= 4.1) ? 
                                                        'green' : (dataMatrix[cell][seg] >= 3.3 && dataMatrix[cell][seg] < 3.5) ? 
                                                            'yellow' : 'red'
                                            }} 
                                            align="center" 
                                            key={"cell_" + seg + "_" + cell}>
                                                {dataMatrix[cell][seg]} V
                                        </TableCell>
                                    ))}
                                </TableRow>
                            ))}
                        </>
                    </TableBody>
                </Table>
            </TableContainer>
            <br />
            <TableContainer component={Paper}>
                <Table sx={{ position: 'relative' }} aria-label="simple table">
                    <TableHead>
                        <TableRow>
                            {Array.from(Array(SEGMENTS).keys()).map((seg) => (
                                <TableCell align="center" key={"header" + seg}><b>Seg. {seg}</b></TableCell>
                            ))}
                        </TableRow>
                    </TableHead>
                    <TableBody>
                        <>
                            {Array.from(Array(TEMPS).keys()).map((temp) => (
                                <TableRow key={temp}>
                                    {Array.from(Array(SEGMENTS).keys()).map((seg) => (
                                        <TableCell 
                                            sx={{
                                                backgroundColor: 
                                                    (dataMatrix[CELLS + temp][seg] >= 20 && dataMatrix[CELLS + temp][seg] < 50) ? 
                                                        'green' : (dataMatrix[CELLS + temp][seg] >= 50 && dataMatrix[CELLS + temp][seg] < 60) ? 
                                                            'yellow' : (dataMatrix[CELLS + temp][seg] >= 0 && dataMatrix[CELLS + temp][seg] < 20) ? 
                                                                'blue' : 'red'
                                            }} 
                                            align="center" 
                                            key={"cell_" + seg + "_" + temp}>
                                                {dataMatrix[CELLS + temp][seg]} °C
                                        </TableCell>
                                    ))}
                                </TableRow>
                            ))}
                        </>
                    </TableBody>
                </Table>
            </TableContainer>
        </>
    )
}

export default Bms