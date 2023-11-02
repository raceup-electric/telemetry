import { Stack } from "@mui/material";
import Grafico from "../components/Grafico";

interface Options {
    identifier: string,
    jsonReferences: string[]
}

interface DefaultPlot {
    jRef: Options[],
    payload: {
        new: { [key: string]: number };
    }
}

// Default plots
function DefaultPlots({ jRef, payload }: DefaultPlot) {
    return (
        <Stack
            justifyContent="center"
            alignItems="center"
            spacing={0}
            sx={{
                width: '95%',
                height: '95%',
                padding: '5%',
                boxSizing: 'border-box'
            }}>
            {jRef.map((el) => (
                <Grafico key={el.identifier.replace(/\s/g, '').toLowerCase()} payload={payload} jsonReference={el.jsonReferences.sort()} title={el.identifier}></Grafico>
            ))}
        </Stack>
    );
}

export default DefaultPlots