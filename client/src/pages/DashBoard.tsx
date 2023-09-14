import { Grid } from "@mui/material"
import Indicatore from "../components/Indicatore"
import { useEffect, useState } from "react";

import { DASHBOARD } from "../log_defs";

interface PayLoad {
    payload: {
        new: { [key: string]: number };
    }
}

function Dashboard({ payload }: PayLoad) {
    const [width, setWidth] = useState(document.body.clientWidth);
    const [height, setHeight] = useState(document.body.clientHeight);
    const setWindowDimensions = () => {
        setWidth(document.body.clientWidth);
        setHeight(document.body.clientHeight);
    }

    useEffect(() => {
        window.addEventListener('resize', setWindowDimensions);
        return () => {
          window.removeEventListener('resize', setWindowDimensions)
        }
    }, [])

    return (
        <>
            <Grid container spacing={2} sx={{ position: 'relative', bottom: 50 }}>
                {DASHBOARD.map((inc) => (
                    <Grid item xs={3} key={inc.reference}>
                        <Indicatore 
                            text={inc.text} 
                            payload={payload}
                            reference={inc.reference} 
                            isText={inc.isText} 
                            unit={inc.unit} 
                            max={inc.max} 
                            cl_width={width/5} 
                            cl_heigth={height/4}></Indicatore>
                    </Grid>
                ))}
            </Grid>
        </>
    )
}

export default Dashboard