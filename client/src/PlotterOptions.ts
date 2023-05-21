export const plotterOptions = [
  {
      value: 'temperature.motors.fl',
      label: 'Front left motor temperature',
      max: 75,
      function: {
        arguments: "value, ref",
        body: "(value < 75) ? document.getElementById(ref)?.setAttribute('style', 'fill: green') : document.getElementById(ref)?.setAttribute('style', 'fill: red');"
      }
    },
    {
      value: 'temperature.motors.fr',
      label: 'Right right motor temperature',
      max: 75,
      function: {
        arguments: "value, ref",
        body: "(value < 75) ? document.getElementById(ref)?.setAttribute('style', 'fill: green') : document.getElementById(ref)?.setAttribute('style', 'fill: red');"
      }
    },
    {
      value: 'temperature.motors.rl',
      label: 'Rear left motor temperature',
      max: 75,
      function: {
        arguments: "value, ref",
        body: "(value < 75) ? document.getElementById(ref)?.setAttribute('style', 'fill: green') : document.getElementById(ref)?.setAttribute('style', 'fill: red');"
      }
    },
    {
      value: 'temperature.motors.rr',
      label: 'Rear right motor temperature',
      max: 75,
      function: {
        arguments: "value, ref",
        body: "(value < 75) ? document.getElementById(ref)?.setAttribute('style', 'fill: green') : document.getElementById(ref)?.setAttribute('style', 'fill: red');"
      }
    },
    {
      value: 'voltage.hv.high',
      label: 'High Voltage high',
    },
    {
      value: 'voltage.hv.avg',
      label: 'High Voltage avg',
    },
    {
      value: 'voltage.hv.low',
      label: 'High Voltage low',
    },
    {
      value: 'voltage.lv.avg',
      label: 'Low Voltage avg',
    },
    {
      value: 'voltage.lv.low',
      label: 'Low Voltage low',
    },
    {
      value: 'car.info.throttle',
      label: 'Throttle',
      function: {
        arguments: "value, ref",
        body: "document.getElementById(ref)?.setAttribute('transform', 'rotate(' + value + ')');"
      }
    },
    {
      value: 'car.info.brake',
      label: 'Brake',
      function: {
        arguments: "value, ref",
        body: "document.getElementById(ref)?.setAttribute('transform', 'rotate(' + value + ')');"
      }
    },
    {
      value: 'car.info.steeringangle',
      label: 'Low Voltage low',
      function: {
        arguments: "value, ref",
        body: "document.getElementById(ref)?.setAttribute('transform', 'rotate(' + value + ')');"
      }
    },
];

export const optionsGroup = [
  {
    id: 'temperature.motors.',
    label: 'Temperatura motori',
    values: [
      'temperature.motors.fl',
      'temperature.motors.fr',
      'temperature.motors.rl',
      'temperature.motors.rr'
    ],
  },
  {
    id: 'voltage.hv.',
    label: 'Alta tensione',
    values: [
      'voltage.hv.high',
      'voltage.hv.avg',
      'voltage.hv.low'
    ],
    max: 75,
    function: "color"
  },
  {
    id: 'car.info.',
    label: 'Info',
    values: [
      'car.info.throttle',
      'car.info.brake',
      'car.info.steeringangle',
    ]
  }
];