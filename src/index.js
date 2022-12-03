import "./style.scss";

import {MDCRipple} from '@material/ripple';
import {MDCFormField} from '@material/form-field';
import {MDCCheckbox} from '@material/checkbox';

import {Modal} from "bootstrap";

const fabRipple = new MDCRipple(document.querySelector('.mdc-fab'));
const checkbox = new MDCCheckbox(document.querySelector('.mdc-checkbox'));
const formField = new MDCFormField(document.querySelector('.mdc-form-field'));
formField.input = checkbox;
const myModal = new Modal('#optionsModal', {
    keyboard: true
})

window.plotArray = [];

/*document.getElementById('newChart').addEventListener('click', () => {

});*/

document.getElementById('initChart').addEventListener('click', () => {
    let xx = [];
    let traces = [];
    let ySelected = [];

    let yChecks = document.getElementById("yOptions").getElementsByClassName('mdc-checkbox');
    for (let i = 0; i < yChecks.length; i++) {
        let elem = yChecks[i].getElementsByTagName('input')[0];
        if (elem.checked && elem.getAttribute('data-indeterminate') === null)
            ySelected.push(elem);
    }

    if (ySelected.length > 0) {
        let id = document.getElementById('grafici').getElementsByClassName('chart').length;
        let newChart = document.createElement('div');
        newChart.id = "chart" + id;
        newChart.className = 'chart';

        newChart.draggable = true;

        document.getElementById('grafici').insertBefore(newChart, document.getElementById('grafici').firstChild);

        ySelected.forEach((element) => {
            let val = element.id;
            xx.push(val);
            traces.push({
                x: [],
                y: [],
                name: val
            });
        });

        plotArray.push([id, newChart, xx, []]); //[id del plot, HTML element, xs, ys]

        Plotly.newPlot(
            newChart,
            traces,
            {
                margin: {
                    t: 0
                },
            }
        );
    }

    myModal.hide();
});