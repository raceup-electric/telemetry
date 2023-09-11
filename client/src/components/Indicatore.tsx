import Plot from 'react-plotly.js';

import {
    Box,
    Card,
    CardContent,
    Typography
} from "@mui/material";
import { useContext, useState } from 'react';
import { SB_CONTEXT } from '../main';

interface Indicator {
    text: string,
    isText: boolean,
    reference: string,
    unit: string,
    max: number,
    cl_width: number,
    cl_heigth: number
}

interface Payload {
    new: { [key: string]: number };
}

function Indicatore({ text, isText, reference, unit, max, cl_width, cl_heigth }: Indicator) {
    const supabase = useContext(SB_CONTEXT)!;

    const [value, setValue] = useState(0);

    // Supabase event
    const ecu = supabase.channel('custom-insert-channel').on('postgres_changes', { 
        event: 'INSERT', 
        schema: 'public', 
        table: 'ecu' 
      },
      (payload) => {
        if(reference === "amk_s") setValue(Math.max(payload.new["amk_status_fl"], payload.new["amk_status_fr"], payload.new["amk_status_rl"], payload.new["amk_status_rr"]))
        else if (reference === "temp_mot") setValue(Math.max(payload.new["temp_motor_fl"], payload.new["temp_motor_fr"], payload.new["temp_motor_rl"], payload.new["temp_motor_rr"]))
        else if (reference === "temp_inv") setValue(Math.max(payload.new["temp_inverter_fl"], payload.new["temp_inverter_fr"], payload.new["temp_inverter_rl"], payload.new["temp_inverter_rr"]))
        else setValue((payload as Payload).new[reference]);
      }
    ).subscribe();

    let content = <div></div>;

    if(isText || (!isText && cl_width < 200)) content = <Typography variant="h5" component="div">{value} {unit}</Typography>;
    else {
        content = <Plot
            data={[
                {
                    domain: { x: [0, 1], y: [0, 1] },
                    value: value,
                    title: { text: text },
                    type: "indicator",
                    mode: "gauge+number",
                    gauge: {
                        axis: { range: [null, max] }
                    }
                }
            ]}
            layout={{width: cl_width, height: cl_heigth, margin: { t: 30, b: 0 }}}
        ></Plot>
    }

    return (
        <Box sx={{ minWidth: '100%', minHeight: '100%', userSelect: 'none' }}>
            <Card variant="outlined">
                <CardContent>
                    <Typography sx={{ fontSize: 14 }} color="text.secondary" gutterBottom>
                        {text}
                    </Typography>
                    {content}
                </CardContent>
            </Card>
        </Box>
    )
}

export default Indicatore