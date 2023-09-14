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
        <div className='innerBody'>
            {jRef.map((el) => (
                <Grafico key={el.identifier.replace(/\s/g, '').toLowerCase()} payload={payload} jsonReference={el.jsonReferences.sort()} title={el.identifier}></Grafico>
            ))}
        </div>
    );
}

export default DefaultPlots