import { useEffect, useState } from "react";
import Plot from "react-plotly.js";

interface Payload {
    new: { [key: string]: number };
}

interface PlotData {
    x: number[],
    y: number[],
    mode: string,
    name: string
}

interface PlotProps {
    jsonReference: string[],
    title: string,
    payload: Payload
}

// Max displayable points
const MAX_POINT = 150;

function Grafico({ jsonReference, title, payload }: PlotProps) {
    const [width, setWidth] = useState(document.body.clientWidth*4/6);
    const [height, setHeight] = useState(document.body.clientHeight*4/6);
    const setWindowDimensions = () => {
        setWidth(document.getElementById("plotContainer")?.clientWidth!);
        setHeight(document.getElementById("plotContainer")?.clientHeight! + 100);
    }

    useEffect(() => {
        window.addEventListener('resize', setWindowDimensions);
        return () => {
          window.removeEventListener('resize', setWindowDimensions)
        }
    }, [])

    const [data, setData] = useState(() => {
        let tempData = [] as PlotData[];
        jsonReference.forEach((ref) => {
            let trace = {
                x: [],
                y: [],
                mode: 'lines+markers',
                name: ref
            }
            tempData.push(trace);
        });

        return tempData;
    });

    let layout = {
        title: title,
        width: width,
        height: height,
        margin: { t: 30, b: 100 },
        showlegend: true
    };

    useEffect(() => {
        let _data = [] as PlotData[];

        data.forEach((d) => {
            if (typeof payload.new[d.name] === 'undefined') return;

            _data.push({
                x: [...d.x, payload.new["millis"]],
                y: [...d.y, payload.new[d.name]],
                mode: 'lines+markers',
                name: d.name
            });
        });

        setData(_data);
    }, [payload]);

    return (
        <>
            <div id="plotContainer" className="plot">
                <Plot
                    data={data}
                    layout={layout}
                ></Plot>
            </div>
        </>
    );
}

export default Grafico