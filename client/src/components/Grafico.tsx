import { useContext, useEffect } from "react";
import { SocketContext } from "../main";
import uPlot, { AlignedData, Range } from "uplot";
import { plotterOptions } from "../PlotterOptions";

interface PlotProps {
    jsonReference: string[],
    title: string,
    custom: boolean
    _range: number[] | undefined
}

const MAX_POINT = 150;
const COLORS: String[] = [
    "FF0000", "00FF00", "0000FF", "FFFF00", "FF00FF", "00FFFF",
    "800000", "008000", "000080", "808000", "800080", "008080",
    "C0C0C0", "808080", "FF8000", "800040", "FF0080", "FF80FF",
    "004080", "FFC0C0", "FFD700", "FF00C0", "00FFC0", "C0FF00",
    "FF40FF", "004040", "FF4000", "408080", "40FF00", "4000FF",
    "40C0FF", "40FFC0", "FF40C0", "C040FF", "C0FF40", "FFC040"
];

function Grafico({ jsonReference, title, custom, _range }: PlotProps) {
    // Resize event for plots
    window.addEventListener("resize", () => {
        grafico.setSize({
            width: document.getElementById("plotContainer")?.clientWidth!,
            height: document.getElementById("plotContainer")?.clientHeight!,
        });
    });

    // SocketIo handler
    const socket = useContext(SocketContext)!;
    
    // Init data with an array for x-points
    let data: AlignedData = [[]];

    // Plot container
    let div = document.createElement("div");
    div.style.marginBottom = "5%";
    
    // Init array for plot data
    let _series: uPlot.Series[] = [{
        label: "Time"
    }];
    let i = 0;
    plotterOptions.forEach(opt => {
        if (!jsonReference.includes(opt.value)) return;
        // Add an array foreach option to plot
        (data as [[], []]).push([])
        _series.push({
            label: opt.value,
            paths: uPlot.paths.spline!(),
            points: {
                show: true,
            },
            stroke: "#" + COLORS[i],
        });
        i++;
    });

    // Plot options
    let opt = {
        title: title,
        width: 800,
        height: 600,
        pxAlign: 0,
        series: _series,
        scales: {
            x: {
                time: false
            },
            y: {
                auto: (custom),
                range: (_range as Range.MinMax),
            }
        }
    }

    // Actual plot
    let grafico = new uPlot(opt, data, div);
    let pointsPlotted = 0;

    // Append plot to page
    useEffect(() => {
        document.getElementById("plotContainer")?.appendChild(div);
        grafico.setSize({
            width: document.getElementById("plotContainer")!.clientWidth, 
            height: document.getElementById("plotContainer")!.clientHeight
        });
    }, []);

    // Socket event on new data
    socket.on("data", (incomingData) => {
        let newPlotData = grafico.data; 
        jsonReference.forEach((ref, i) => {
            // Get data from json
            let value = ((ref).split(".")).reduce((a, c) => a[c], incomingData);

            // Add y-data to series
            (newPlotData[i+1] as Number[]).push(value);

            // Add x
            if(i == 0) (newPlotData[0] as Number[]).push(Number(incomingData.timestamp));
        });

        // Start sliding
        if(pointsPlotted >= MAX_POINT){
            newPlotData.map(el => (el as Number[]).shift())
        }

        grafico.setData(newPlotData);
        pointsPlotted++;

        //Status 
        // RSSI -> -50:-130
        // SNR -> 10:-20
        if(incomingData.RSSI < -130)
            document.getElementById("rssi")!.style.color = 'red';
        if(incomingData.RSSI >= -130 && incomingData.RSSI < -85)
            document.getElementById("rssi")!.style.color = 'yellow';
        if(incomingData.RSSI >= -85)
            document.getElementById("rssi")!.style.color = 'green';

        if(incomingData.SNR < -20)
            document.getElementById("snr")!.style.color = 'red';
        if(incomingData.SNR >= -20 && incomingData.SNR < -5)
            document.getElementById("snr")!.style.color = 'yellow';
        if(incomingData.SNR >= -5)
            document.getElementById("snr")!.style.color = 'green';
    });

    return (
        <div id="plotContainer" className="plot" />
    );
}

export default Grafico