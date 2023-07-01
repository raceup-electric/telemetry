import Grafico from "../components/Grafico";

function PreloadedPlots({ jRef }: any) {
    return (
        <div className='innerBody'>
            {jRef.map(el => (
                <Grafico jsonReference={el.values} title={el.label} custom={false} key={el.id}></Grafico>
            ))}
        </div>
    );
}

export default PreloadedPlots