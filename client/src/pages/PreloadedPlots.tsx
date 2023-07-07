import Grafico from "../components/Grafico";

interface PreloadedPlot {
    values: string[],
    label: string
    range: number[],
    custom: boolean
}

// Default plots
function PreloadedPlots({ jRef }: any) {
    return (
        <div className='innerBody'>
            {jRef.map((el: PreloadedPlot) => (
                <Grafico jsonReference={el.values} title={el.label} _range={el.range} custom={false} key={el.label}></Grafico>
            ))}
        </div>
    );
}

export default PreloadedPlots