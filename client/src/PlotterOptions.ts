export const plotterOptions = [
  // Temperature motori
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

    // Temperature IGBT
    {
      value: 'temperature.IGBT.fl',
      label: 'Front left IGBT temperature',
    },
    {
      value: 'temperature.IGBT.fr',
      label: 'Front right IGBT temperature',
    },
    {
      value: 'temperature.IGBT.rl',
      label: 'Rear left IGBT temperature',
    },
    {
      value: 'temperature.IGBT.rr',
      label: 'Rear right IGBT temperature',
    },

    // Temperature inverter
    {
      value: 'temperature.inverter.fl',
      label: 'Front left inverter temperature',
    },
    {
      value: 'temperature.inverter.fr',
      label: 'Front right inverter temperature',
    },
    {
      value: 'temperature.inverter.rl',
      label: 'Rear left inverter temperature',
    },
    {
      value: 'temperature.inverter.rr',
      label: 'Rear right inverter temperature',
    },

    // Stato motori
    {
      value: 'status.motor.fl',
      label: 'Front left motor status',
    },
    {
      value: 'status.motor.fr',
      label: 'Front right motor status',
    },
    {
      value: 'status.motor.rl',
      label: 'Rear left motor status',
    },
    {
      value: 'status.motor.rr',
      label: 'Rear right motor status',
    },

    // Stato macchina
    {
      value: 'car_status',
      label: 'Car status',
    },

    // HV
    {
      value: 'voltage.hv.max',
      label: 'High Voltage max voltage',
    },
    {
      value: 'voltage.hv.min',
      label: 'High Voltage min voltage',
    },
    {
      value: 'voltage.hv.mean',
      label: 'High Voltage mean voltage',
    },
    {
      value: 'temp.hv.max',
      label: 'High Voltage max temperature',
    },
    {
      value: 'temp.hv.min',
      label: 'High Voltage min temperature',
    },
    {
      value: 'temp.hv.mean',
      label: 'High Voltage min temperature',
    },

    // LV
    {
      value: 'voltage.lv.tot',
      label: 'Low Voltage tot voltage',
    },
    {
      value: 'voltage.lv.min',
      label: 'Low Voltage min voltage',
    },
    {
      value: 'temp.lv.max',
      label: 'Low Voltage max temperature',
    },

    // Dx
    {
      value: 'temp.dx.precold',
      label: 'Dx precold temp',
    },
    {
      value: 'temp.dx.postcold',
      label: 'Dx postcold temp',
    },
    {
      value: 'temp.dx.premot',
      label: 'Dx premot temp',
    },
    {
      value: 'temp.dx.postmot',
      label: 'Dx postmot temp',
    },

    // Sx
    {
      value: 'temp.sx.precold',
      label: 'Dx precold temp',
    },
    {
      value: 'temp.sx.postcold',
      label: 'Dx postcold temp',
    },
    {
      value: 'temp.sx.premot',
      label: 'Dx premot temp',
    },
    {
      value: 'temp.sx.postmot',
      label: 'Dx postmot temp',
    },
];

export const optionsGroup = [
  {
    id: 'HV',
    label: 'HV',
    values: [
      {
        id: 'voltage.hv.',
        label: 'Tensione HV',
        values: [
          'voltage.hv.max',
          'voltage.hv.min',
          'voltage.hv.mean',
        ],
      },
      {
        id: 'temp.hv.',
        label: 'Temperature HV',
        values: [
          'temp.hv.max',
          'temp.hv.min',
        ],
      }
    ]
  },
  {
    id: 'LV',
    label: 'LV',
    values: [
      {
        id: 'voltage.lv.',
        label: 'Tensione LV',
        values: [
          'voltage.lv.tot',
          'voltage.lv.min',
        ],
      },
      {
        id: 'temp.lv.',
        label: 'Temperature LV',
        values: [
          'temp.lv.max',
        ],
      }
    ]
  },
  {
    id: 'Power',
    label: 'Power',
    values: [
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
        id: 'temperature.IGBT.',
        label: 'Temperatura IGBT',
        values: [
          'temperature.IGBT.fl',
          'temperature.IGBT.fr',
          'temperature.IGBT.rl',
          'temperature.IGBT.rr'
        ],
      },
      {
        id: 'temperature.inverter.',
        label: 'Temperatura inverter',
        values: [
          'temperature.inverter.fl',
          'temperature.inverter.fr',
          'temperature.inverter.rl',
          'temperature.inverter.rr'
        ],
      }
    ]
  },
  {
    id: 'Cooling',
    label: 'Cooling',
    values: [
      {
        id: 'temp.dx.',
        label: 'Temperature pre/post dx',
        values: [
          'temp.dx.precold',
          'temp.dx.postcold',
          'temp.dx.premot',
          'temp.dx.postmot',
        ],
      },
      {  
        id: 'temp.sx.',
        label: 'Temperature pre/post sx',
        values: [
          'temp.sx.precold',
          'temp.sx.postcold',
          'temp.sx.premot',
          'temp.sx.postmot',
        ],
      }
    ]
  },
];