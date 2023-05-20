import { useContext, useState, useEffect } from "react";
import { SocketContext } from "../main";
import uPlot from "uplot";
import { optionsGroup, plotterOptions } from "../PlotterOptions";

interface PlotProps {
    jsonReference: string[],
    title: string,
    custom: boolean
}

const MAX_POINT = 60;

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
    //div.className = "plot";
    div.id = "plot"
    
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
        width: 800,//document.getElementById("plot")!.clientWidth,
        height: 600,//document.getElementById("plot")!.clientHeight,
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

            if(pointsPlotted >= MAX_POINT){
                if(i == 0) data1[0].push(pointsPlotted + 1);
            }
            
            if(!custom) {
                optionsGroup.forEach(group => {
                    if (group.values.includes(ref)) {
                        value < group.max ?
                            document.getElementById(ref)?.setAttribute('style', 'fill: green') :
                            document.getElementById(ref)?.setAttribute('style', 'fill: red');
                    }
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
    });

    return (
        <div id="plotContainer" className="plot" />
    );
}

export default Grafico