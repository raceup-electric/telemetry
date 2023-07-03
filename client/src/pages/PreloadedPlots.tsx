import Grafico from "../components/Grafico";

interface PreloadedPlot {
    values: string[],
    label: string
}

// Default plots
function PreloadedPlots({ jRef }: any) {
    return (
        <div className='innerBody'>
            {jRef.map((el: PreloadedPlot) => (
                <Grafico jsonReference={el.values} title={el.label} key={el.label}></Grafico>
            ))}
        </div>
    );
}

export default PreloadedPlots