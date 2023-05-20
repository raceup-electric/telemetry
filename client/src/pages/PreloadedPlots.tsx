import Grafico from "../components/Grafico";
import Car from "../components/svg/car";
import HV from "../components/svg/hv";

interface DefaultPlot {
    jRef: string[],
    page: string,
    title: string
}

function PreloadedPlots({ jRef, page, title }: DefaultPlot) {
    return (
        <div className='innerBody'>
            <Grafico jsonReference={jRef} title={title} custom={false}  />
            {(page.startsWith("temperature.motors")) ? <Car /> : <></>}
            {(page.startsWith("voltage.hv")) ? <HV /> : <></>}
        </div>
    );
}

export default PreloadedPlots