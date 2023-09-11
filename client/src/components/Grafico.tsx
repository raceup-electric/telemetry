import { useContext, useEffect } from "react";
import uPlot, { AlignedData, Range } from "uplot";
import { SB_CONTEXT } from "../main";

interface PlotProps {
    jsonReference: string[],
    title: string
}

interface Payload {
    new: { [key: string]: number };
}

// Max displayable points
const MAX_POINT = 150;

// Line colors
const COLORS: String[] = [
    "FF0000", "00FF00", "0000FF", "FFFF00", "FF00FF", "00FFFF",
    "800000", "008000", "000080", "808000", "800080", "008080",
    "C0C0C0", "808080", "FF8000", "800040", "FF0080", "FF80FF",
    "004080", "FFC0C0", "FFD700", "FF00C0", "00FFC0", "C0FF00",
    "FF40FF", "004040", "FF4000", "408080", "40FF00", "4000FF",
    "40C0FF", "40FFC0", "FF40C0", "C040FF", "C0FF40", "FFC040"
];

function Grafico({ jsonReference, title }: PlotProps) {
    const supabase = useContext(SB_CONTEXT)!;
    
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
    jsonReference.forEach(opt => {
        // Add an array foreach option to plot
        (data as [[], []]).push([]);
        _series.push({
            label: opt,
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
                auto: true
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

    // Supabase event
    const ecu = supabase.channel('custom-insert-channel').on('postgres_changes', { 
        event: 'INSERT', 
        schema: 'public', 
        table: 'ecu' 
      },
      (payload) => {
        let newPlotData = grafico.data; 
        jsonReference.forEach((ref, i) => {
            // Add y-data to series
            (newPlotData[i+1] as Number[]).push((payload as Payload).new[ref]);

            // Add x
            if(i == 0) (newPlotData[0] as Number[]).push(Number(payload.new["timestamp"]));
        });

        // Start sliding
        if(pointsPlotted >= MAX_POINT) newPlotData.map(el => (el as Number[]).shift())

        grafico.setData(newPlotData);
        pointsPlotted++;
      }
    ).subscribe();

    return (
        <>
            <div id="plotContainer" className="plot" />
        </>
    );
}

export default Grafico