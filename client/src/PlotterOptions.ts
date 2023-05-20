export const plotterOptions = [
    {
      value: 'temperature.motors.fl',
      label: 'Front left motor temperature',
    },
    {
      value: 'temperature.motors.fr',
      label: 'Right right motor temperature',
    },
    {
      value: 'temperature.motors.rl',
      label: 'Rear left motor temperature',
    },
    {
      value: 'temperature.motors.rr',
      label: 'Rear right motor temperature',
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
    },
    {
      value: 'car.info.brake',
      label: 'Brake',
    },
    {
      value: 'car.info.steerengangle',
      label: 'Low Voltage low',
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
    max: 75
  },
  {
    id: 'voltage.hv.',
    label: 'Alta tensione',
    values: [
      'voltage.hv.high',
      'voltage.hv.avg',
      'voltage.hv.low'
    ],
    max: 75
  }
];