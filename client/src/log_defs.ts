export const LOG_DEFS = [
  {
    id: 'tempmotor',
    label: 'Temperatura motori',
    values: [
      {
        identifier: 'Temperatura motori',
        jsonReferences: [
          'temperature_motor_fl',
          'temperature_motor_fr',
          'temperature_motor_rl',
          'temperature_motor_rr'
        ],
        range: [10, 90],
        unit: "T"
      },
      {
        identifier: 'Temperatura IGBT',
        jsonReferences: [
          'temperature_igbt_fl',
          'temperature_igbt_fr',
          'temperature_igbt_rl',
          'temperature_igbt_rr'
        ],
        range: [10, 90],
        unit: "T"
      },
      {
        identifier: 'Temperatura inverter',
        jsonReferences: [
          'temperature_inverter_fl',
          'temperature_inverter_fr',
          'temperature_inverter_rl',
          'temperature_inverter_rr'
        ],
        range: [10, 90],
        unit: "T"
      }
    ]
  },
  {
    id: 'torque',
    label: 'Torque limits',
    values: [
      {
        identifier: 'Torque positivo',
        jsonReferences: [
          'torque_limit_positive_fl',
          'torque_limit_positive_fr',
          'torque_limit_positive_rl',
          'torque_limit_positive_rr'
        ],
        range: [10, 90],
        unit: "Nm"
      },
      {
        identifier: 'Torque negativo',
        jsonReferences: [
          'torque_limit_negative_fl',
          'torque_limit_negative_fr',
          'torque_limit_negative_rl',
          'torque_limit_negative_rr'
        ],
        range: [10, 90],
        unit: "Nm"
      }
    ]
  },
  {
    id: 'guida',
    label: 'Interfacce pilota',
    values: [
      {
        identifier: 'Acceleratore',
        jsonReferences: [
          'throttle',
          'actual_velocity_kmh',
          'thr_req',
          'acc_pot_1',
          'acc_pot_2'
        ]
      },
      {
        identifier: 'Freno',
        jsonReferences: [
          'brake',
          'brake_press',
          'brk_req',
          'brk_pot',
        ]
      },
      {
        identifier: 'Volante',
        jsonReferences: [
          'steering_angle'
        ]
      }
    ]
  },
  {
    id: 'power',
    label: 'Potenze',
    values: [
      {
        identifier: 'Tensione macchina - corrente - potenza',
        jsonReferences: [
          'car_voltage',
          'lem_current',
          'current_sens',
          'total_power'
        ]
      }
    ]
  },
  {
    id: 'accelerations',
    label: 'Accelerazioni',
    values: [
      {
        identifier: 'Accelerazioni',
        jsonReferences: [
          'acceleration_x',
          'acceleration_y',
          'acceleration_z'
        ],
        range: [-20, 20],
        unit: "ms^2"
      },
      {
        identifier: 'Omega',
        jsonReferences: [
          'omega_x',
          'omega_y',
          'omega_z'
        ],
        unit: "deg"
      }
    ]
  },
  {
    id: 'suspensions',
    label: 'Sospensioni',
    values: [
      {
        identifier: 'Sospensioni',
        jsonReferences: [
          'suspensions_fl',
          'suspensions_fr',
          'suspensions_rl',
          'suspensions_rr'
        ]
      }
    ]
  },
  {
    id: 'temperatures',
    label: 'Temperature',
    values: [
      {
        identifier: 'Temperature',
        jsonReferences: [
          'temp_0',
          'temp_1',
          'temp_2',
          'temp_3',
          'temp_4',
          'temp_5',
          'temp_6',
          'temp_7',
        ]
      }
    ]
  },
  {
    id: 'ventoles',
    label: 'Ventole',
    values: [
      {
        identifier: 'Ventole',
        jsonReferences: [
          'fan_speed_r',
          'fan_speed_l'
        ]
      }
    ]
  }
];

export const DASHBOARD = [
  {
    text: "Throttle",
    reference: "throttle",
    isText: false,
    unit: "%",
    max: 100
  },
  {
    text: "Brake",
    reference: "brake",
    isText: false,
    unit: "%",
    max: 100
  },
  {
    text: "Velocità",
    reference: "actual_velocity_kmh",
    isText: false,
    unit: "km/h",
    max: 150
  },
  {
    text: "Power",
    reference: "total_power",
    isText: false,
    unit: "kW",
    max: 80
  },
  {
    text: "Amk status",
    reference: "amk_s",
    isText: true,
    unit: "",
    max: 100
  },
  {
    text: "Status",
    reference: "status",
    isText: true,
    unit: "",
    max: 100
  },
  {
    text: "BMS map",
    reference: "bms_error_map",
    isText: true,
    unit: "",
    max: 100
  },
  {
    text: "Hottest slave",
    reference: "max_temp_n_slave",
    isText: true,
    unit: "",
    max: 100
  },
  {
    text: "Max volt HV",
    reference: "max_volt_hv",
    isText: true,
    unit: "V",
    max: 4.3
  },
  {
    text: "Min volt HV",
    reference: "min_volt_hv",
    isText: true,
    unit: "V",
    max: 4.3
  },
  {
    text: "Max volt LV",
    reference: "max_volt_lv",
    isText: true,
    unit: "V",
    max: 4.3
  },
  {
    text: "Min volt LV",
    reference: "min_volt_lv",
    isText: true,
    unit: "V",
    max: 4.3
  },
  {
    text: "Max temp inverter",
    reference: "temp_inv",
    isText: true,
    unit: "°C",
    max: 100
  },
  {
    text: "Max temp motori",
    reference: "temp_mot",
    isText: true,
    unit: "°C",
    max: 100
  },
  {
    text: "Max temp LV",
    reference: "max_temp_lv",
    isText: true,
    unit: "°C",
    max: 50
  },
  {
    text: "Max temp HV",
    reference: "max_temp_hv",
    isText: true,
    unit: "°C",
    max: 50
  },
]