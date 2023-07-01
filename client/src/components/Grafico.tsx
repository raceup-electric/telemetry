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
    
    let data = [Array.from(Array(MAX_POINT).keys())];
    
    let div = document.createElement("div");
    div.style.marginBottom = "5%";
    //div.id = "plot"
    
    let series: uPlot.Series[] = [{
        label: "Time"
    }];
    plotterOptions.forEach(opt => {
        if (!jsonReference.includes(opt.value)) return;
        data.push([]);
        series.push({
            label: opt.value,
            paths: uPlot.paths.spline!(),
            points: {
                show: true,
            },
            stroke: "#" + String(Math.floor(Math.random() * 16777215).toString(16)),
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
            let value = ((ref).split(".")).reduce((a, c) => a[c], data);

            data1[i+1].push(value);

            if(pointsPlotted >= MAX_POINT && i == 0) data1[0].push(pointsPlotted + 1);
            
            if(!custom) {
                plotterOptions.forEach(opt => {
                    if(opt.value != ref) return;
                    try {
                        let refFunction = new Function(opt.function!.arguments, opt.function!.body);
                        refFunction(value, ref);
                    } catch (err) {};
                });
            }
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
        if(data.RSSI > -130 && data.RSSI < -85)
            document.getElementById("rssi")!.style.color = 'yellow';
        if(data.RSSI > -85)
            document.getElementById("rssi")!.style.color = 'green';

        if(data.SNR < -20)
            document.getElementById("snr")!.style.color = 'red';
        if(data.RSSI > -20 && data.RSSI < -5)
            document.getElementById("snr")!.style.color = 'yellow';
        if(data.RSSI > -5)
            document.getElementById("snr")!.style.color = 'green';
    });

    return (
        <div id="plotContainer" className="plot" />
    );
}

export default Grafico