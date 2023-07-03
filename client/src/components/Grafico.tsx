import { useContext, useEffect } from "react";
import { SocketContext } from "../main";
import uPlot, { AlignedData, TypedArray } from "uplot";
import { plotterOptions } from "../PlotterOptions";

interface PlotProps {
    jsonReference: string[],
    title: string,
}

const MAX_POINT = 150;
const COLORS: String[] = [
    "88CCEE", 
    "CC6677", 
    "DDCC77", 
    "117733", 
    "332288", 
    "AA4499", 
    "44AA99", 
    "999933", 
    "882255", 
    "661100",
    "6699CC",
    "888888"
];

function Grafico({ jsonReference, title }: PlotProps) {
    // Resize event for plots
    function getSize() {
        return {
            width: document.getElementById("plotContainer")?.clientWidth!,
            height: document.getElementById("plotContainer")?.clientHeight!,
        }
    }
    window.addEventListener("resize", e => {
        grafico.setSize(getSize());
    });

    // SocketIo handler
    const socket = useContext(SocketContext)!;
    
    // Init data with an array for x-points
    let data: AlignedData = [[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[]];

    // Plot container
    let div = document.createElement("div");
    div.style.marginBottom = "5%";
    
    // Init array for plot data
    let series: uPlot.Series[] = [{
        label: "Time"
    }];
    let i = 0;
    plotterOptions.forEach(opt => {
        if (!jsonReference.includes(opt.value)) return;
        // Add an array foreach option to plot
        //data.push([]);
        series.push({
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
        series: series,
        scales: {
            x: {
                time: false
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
    socket.on("data", (data) => {
        let data1 = grafico.data; 
        jsonReference.forEach((ref, i) => {
            // Get data from json
            let value = ((ref).split(".")).reduce((a, c) => a[c], data);

            // Add y-data to series
            (data1[i+1] as Number[]).push(value);

            // Add x
            if(i == 0) (data1[0] as Number[]).push(Number(data.timestamp));
        });

        // Start sliding
        if(pointsPlotted >= MAX_POINT){
            data1.map(el => (el as Number[]).shift())
        }

        grafico.setData(data1);
        pointsPlotted++;

        //Status 
        // RSSI -> -50:-130
        // SNR -> 10:-20
        if(data.RSSI < -130)
            document.getElementById("rssi")!.style.color = 'red';
        if(data.RSSI >= -130 && data.RSSI < -85)
            document.getElementById("rssi")!.style.color = 'yellow';
        if(data.RSSI >= -85)
            document.getElementById("rssi")!.style.color = 'green';

        if(data.SNR < -20)
            document.getElementById("snr")!.style.color = 'red';
        if(data.SNR >= -20 && data.SNR < -5)
            document.getElementById("snr")!.style.color = 'yellow';
        if(data.SNR >= -5)
            document.getElementById("snr")!.style.color = 'green';
    });

    return (
        <div id="plotContainer" className="plot" />
    );
}

export default Grafico