import { MAX_POINT } from "../Plot"
import Plot from "../Plot";
import uPlot from 'uplot';

import { useEffect } from 'react'
import Car from "../components/svg/car";

function PreloadedPlots({ page, title, addPlot }: any) {
    useEffect(() => {
        let data = [Array.from(Array(MAX_POINT).keys())];
        
        let opts = Plot.defaultPlot(page, title);
        opts.series.forEach(() => data.push([]));
    
        let newPlot = new uPlot(opts, data, document.getElementById("plot"));
        addPlot(newPlot);
    }, []);

    return (
        <div className='innerBody'>
            <div id="plot" className="plot"></div>
            {(page.startsWith("temperature.moto")) ? <Car /> : <></>}
        </div>
    );
}

export default PreloadedPlots