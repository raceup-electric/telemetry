import "./style.scss";

window.Plotly = require('plotly.js-dist-min');
window.io = require('socket.io-client');

import {Modal, Button} from "bootstrap";

const myModal1 = new Modal('#plotModal', {
    keyboard: true
});
const myModal2 = new Modal('#indicatorsModal', {
    keyboard: true
})

window.plotArray = [];

document.getElementById('initChart').addEventListener('click', () => {
    let yChecks = document.getElementById("optionsPlot").querySelectorAll('input[type=checkbox]:checked')

    if (yChecks.length > 0) {
        let id = document.getElementById('grafici').getElementsByClassName('chart').length;
        let newChart = document.createElement('div');
        newChart.id = "chart" + id;
        newChart.className = 'chart';
        newChart.draggable = true;
        document.getElementById('grafici').insertBefore(newChart, document.getElementById('grafici').firstChild);

        let y = [];
        let traces = [];

        yChecks.forEach((element) => {
            y.push(element.id);
            traces.push({
                x: [],
                y: [],
                name: element.id
            });
        });

        plotArray.push([newChart.id, newChart, y]); //[id del plot, HTML element, xs, ys]

        Plotly.newPlot(
            newChart,
            traces,
            {
                plot_bgcolor: "#121212",
                paper_bgcolor: "#121212",
                xaxis: {
                    color: "#ededed"
                },
                yaxis: {
                    color: "#ededed"
                },
                legend: {
                    font: {
                        color: "#ededed"
                    },
                },
                showlegend: true,
                margin: {
                    t: 0
                }
            }
        );
    }

    myModal1.hide();
});

document.getElementById('initIndicator').addEventListener('click', () => {
    let element = document.getElementById("indicatorPlot").querySelector('input[type=radio]:checked')

    element.disabled = true;
    element.checked = false;

    let id = document.getElementById('indicatori').getElementsByClassName('indicator').length;
    let newChart = document.createElement('div');
    newChart.id = "indicator" + id;
    newChart.className = 'indicator';
    newChart.draggable = true;
    document.getElementById('indicatori').insertBefore(newChart, document.getElementById('indicatori').firstChild);

    plotArray.push([newChart.id, newChart, element.id]);

    //TODO: shape

    Plotly.newPlot(
        newChart,
        [
            {
                value: 0,
                title: {
                    text: element.id,
                    font: {
                        size: 14,
                        color: "#ededed"
                    },
                },
                type: "indicator",
                mode: "gauge+number",
                gauge: {
                    axis: {
                        range: [
                            0,
                            350
                        ]
                    }
                }
            }
        ],
        {
            plot_bgcolor: "#121212",
            paper_bgcolor: "#121212",
            autoscale: true,
            width: 250,
            height: 192,
            font: {
                color: "#ededed"
            },
            margin: {
                l: 25,
                r: 25,
                b: 25,
                t: 25,
                pad: 0
            }
        }
    );

    myModal2.hide();
});