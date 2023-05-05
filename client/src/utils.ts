function scaleGradient(u: uPlot, scaleKey: string, ori: number, scaleStops, discrete = false) {
    let can = document.createElement("canvas");
	  let ctx = can.getContext("2d");

    let scale = u.scales[scaleKey];

    let minStopIdx;
    let maxStopIdx;

    for (let i = 0; i < scaleStops.length; i++) {
      let stopVal = scaleStops[i][0];

      if (stopVal <= scale.min || minStopIdx == null)
        minStopIdx = i;

      maxStopIdx = i;

      if (stopVal >= scale.max)
        break;
    }

    if (minStopIdx == maxStopIdx)
      return scaleStops[minStopIdx][1];

    let minStopVal = scaleStops[minStopIdx][0];
    let maxStopVal = scaleStops[maxStopIdx][0];

    if (minStopVal == -Infinity)
      minStopVal = scale.min;

    if (maxStopVal == Infinity)
      maxStopVal = scale.max;

    let minStopPos = u.valToPos(minStopVal, scaleKey, true);
    let maxStopPos = u.valToPos(maxStopVal, scaleKey, true);

    let range = minStopPos - maxStopPos;

    let x0, y0, x1, y1;

    if (ori == 1) {
      x0 = x1 = 0;
      y0 = minStopPos;
      y1 = maxStopPos;
    }
    else {
      y0 = y1 = 0;
      x0 = minStopPos;
      x1 = maxStopPos;
    }

    let grd = ctx.createLinearGradient(x0, y0, x1, y1);

    let prevColor;

    for (let i = minStopIdx; i <= maxStopIdx; i++) {
      let s = scaleStops[i];

      let stopPos = i == minStopIdx ? minStopPos : i == maxStopIdx ? maxStopPos : u.valToPos(s[0], scaleKey, true);
      let pct = (minStopPos - stopPos) / range;

      if (discrete && i > minStopIdx)
        grd.addColorStop(pct, prevColor);

      grd.addColorStop(pct, prevColor = s[1]);
    }

    return grd;
}

export default scaleGradient