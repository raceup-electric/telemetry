import Grafico from "../components/Grafico";

interface Options {
    identifier: string,
    jsonReferences: string[]
}

interface DefaultPlot {
    jRef: Options[]
}

// Default plots
function DefaultPlots({ jRef }: DefaultPlot) {
    return (
        <div className='innerBody'>
            {jRef.map((el) => (
                <Grafico key={el.identifier.replace(/\s/g, '').toLowerCase()} jsonReference={el.jsonReferences.sort()} title={el.identifier}></Grafico>
            ))}
        </div>
    );
}

export default DefaultPlots