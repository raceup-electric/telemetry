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
  
    addPoint() {
      this.pointsPlotted = this.pointsPlotted + 1;
    }
}

export default Plot