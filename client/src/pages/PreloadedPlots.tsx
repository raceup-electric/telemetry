import Grafico from "../components/Grafico";
import Car from "../components/svg/car";
import Info from "../components/svg/info";

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
        </div>
    );
}

export default PreloadedPlots