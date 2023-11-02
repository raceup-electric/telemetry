import Plot from 'react-plotly.js';

import {
    Box,
    Card,
    CardContent,
    Typography
} from "@mui/material";
import { useEffect, useState } from 'react';

interface Payload {
    new: { [key: string]: number };
}

interface Indicator {
    text: string,
    isText: boolean,
    payload: Payload,
    reference: string,
    unit: string,
    max: number,
    cl_width: number,
    cl_heigth: number
}

function Indicatore({ text, isText, payload, reference, unit, max, cl_width, cl_heigth }: Indicator) {
    const [value, setValue] = useState(0);

    useEffect(() => {
        let newValue = 0;

        newValue = (typeof payload.new[reference] === 'undefined') ? 0 : payload.new[reference];
        switch(reference) {
            case 'amk_s':
                newValue = Math.max(
                    (typeof payload.new['amk_status_fl'] === 'undefined') ? -Infinity : payload.new['amk_status_fl'],
                    (typeof payload.new['amk_status_fr'] === 'undefined') ? -Infinity : payload.new['amk_status_fr'],
                    (typeof payload.new['amk_status_rl'] === 'undefined') ? -Infinity : payload.new['amk_status_rl'],
                    (typeof payload.new['amk_status_rr'] === 'undefined') ? -Infinity : payload.new['amk_status_rr'],
                );
        }

        setValue(newValue)
    }, [payload]);

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
            <Card variant="outlined" >
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