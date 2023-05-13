import uPlot from 'uplot';
import { optionsGroup, plotterOptions } from './PlotterOptions';

class Plot {
    container: Element;
    opts;
    jsonReference: String[];
    uplot: uPlot;
    pointsPlotted: number = 0;
  
    constructor(_container: Element, _opts: object, _jsonReference: String[], _plot: uPlot) {
      this.container = _container;
      this.opts = _opts;
      this.jsonReference = [..._jsonReference];
      this.uplot = _plot;
    }
  
    getPointsPlotted() {
      return this.pointsPlotted;
    }
  
    updatePointCounter() {
      this.pointsPlotted = this.pointsPlotted + 1;
    }

    appendPoint(data: any) {
      let _tempData = this.uplot.data;    
      this.jsonReference.forEach((jsonRef, i) => {
        let value = ((jsonRef).split(".")).reduce((a, c) => a[c], data);
        
        _tempData[i+1].push(value);

        if(this.getPointsPlotted() >= MAX_POINT){
          if(i == 0) _tempData[0].push(this.getPointsPlotted() + 1);
        }

        optionsGroup.forEach((group) => {
          if(jsonRef.startsWith(group)) {
            value < group.max ? 
              document.getElementById(jsonRef)?.setAttribute('style', 'fill: green') :
              document.getElementById(jsonRef)?.setAttribute('style', 'fill: red')
          }
        });
      });
      if(this.getPointsPlotted() >= MAX_POINT){
        _tempData.forEach((dat: any) => {
          dat.shift();
        })
      }
      this.uplot.setData(_tempData);
      this.updatePointCounter();
    }

    static defaultPlot(datas: string, title: string) {
      let _series = [{
        label: "Time"
      }];

      plotterOptions.forEach(opt => {
        if (!opt.value.startsWith(datas)) return;
        _series.push({
            label: opt.value,
            paths: uPlot.paths.spline!(),
            points: {
                show: true,
            },
            stroke: "#" + String(Math.floor(Math.random() * 16777215).toString(16)),
        });
      });

      return {
        title: title,
        width: document.getElementById("plot")!.clientWidth,
        height: document.getElementById("plot")!.clientHeight,
        pxAlign: 0,
        series: _series,
        scales: {
            x: {
                time: false
            }
        }
      };
    }
}

export default Plot
export const MAX_POINT = 60;