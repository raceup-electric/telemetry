import { useContext, useState, useEffect } from "react";
import { SocketContext } from "../main";
import uPlot from "uplot";
import { plotterOptions } from "../PlotterOptions";

interface PlotProps {
    jsonReference: string[],
    title: string,
    custom: boolean
}

const MAX_POINT = 150;
const COLORS: String[] = [
    '88CCEE', 
    'CC6677', 
    'DDCC77', 
    '117733', 
    '332288', 
    'AA4499', 
    '44AA99', 
    '999933', 
    '882255', 
    '661100',
    '6699CC',
    '888888'
];

function Grafico({ jsonReference, title, custom }: PlotProps) {
    function getSize() {
        return {
            width: document.getElementById("plotContainer")?.clientWidth!,
            height: document.getElementById("plotContainer")?.clientHeight!,
        }
    }

    window.addEventListener("resize", e => {
        grafico.setSize(getSize());
    });

    const socket = useContext(SocketContext)!;
    
    //let data = [Array.from(Array(MAX_POINT).keys())];
    let data = [[]];
    
    let div = document.createElement("div");
    div.style.marginBottom = "5%";
    //div.id = "plot"
    
    let series: uPlot.Series[] = [{
        label: "Time"
    }];
    plotterOptions.forEach((opt, i) => {
        if (!jsonReference.includes(opt.value)) return;
        data.push([]);
        series.push({
            label: opt.value,
            paths: uPlot.paths.spline!(),
            points: {
                show: true,
            },
            stroke: "#" + COLORS[i],
        });
    });

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

    let grafico = new uPlot(opt, data, div);
    let pointsPlotted = 0;

    useEffect(() => {
        document.getElementById("plotContainer")?.appendChild(div);
        grafico.setSize({
            width: document.getElementById("plotContainer")!.clientWidth, 
            height: document.getElementById("plotContainer")!.clientHeight
        });
    }, []);

    socket.on("data", (data) => {
        let data1 = grafico.data; 
        jsonReference.forEach((ref, i) => {
            // Get data from json
            let value = ((ref).split(".")).reduce((a, c) => a[c], data);

            // Add y-data to series
            data1[i+1].push(value);

            if(/*pointsPlotted >= MAX_POINT &&*/ i == 0) data1[0].push(/*pointsPlotted + 1*/ Number(data.timestamp));
        });
        if(pointsPlotted >= MAX_POINT){
            data1.forEach((dat) => {
                dat.shift();
            })
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