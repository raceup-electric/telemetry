import React, { useContext, useEffect, useState } from 'react'
import { SocketContext } from './main';
import uPlot from 'uplot';
import UplotReact from 'uplot-react';
//import 'uplot/dist/uPlot.min.css';
import { mockData } from './MOCK_DATA-3'

interface CarData {
  voltage: number
}

let len = 1;
let start = 0;

class ClassApp extends React.Component<Record<string, never>, { options: uPlot.Options; data: uPlot.AlignedData }> {
  constructor(args: Record<string, never>) {
    super(args);
    this.state = {
      options: {
        title: "Chart",
        width: 800,
        height: 500,
        series: [
          {
            label: "Date"
          },
          {
            label: "",
            points: { show: false },
            stroke: "blue",
            fill: "blue"
          }
        ],
        scales: { x: { time: false } }
      },
      data: [
        mockData[0].slice(start, len),
        mockData[1].slice(start, len)
      ]
    };      
  }
  
  render(): React.ReactNode {
    return (
      <UplotReact
      key="class-key"
      options={this.state.options}
      data={this.state.data}
      // Uncomment the line below to use predefined target
      // target={root}
      onDelete={(/* chart: uPlot */) => console.log("Deleted from class")}
      onCreate={(/* chart: uPlot */) => console.log("Created from class")}
      />
      );
    }
  }
  
  function App() {
    const [ data, setData ] = useState<CarData>();
    
    const socket = useContext(SocketContext)!;
    
    socket.on("connect", () => {
      console.log("Connected to SocketIO");
    })
  
  socket.on("hello", (newData) => {
    setData(JSON.parse(newData));

    /*len++;
    if(len > 100) start++;
    const options = {
      ...this.state.options,
      title: "Rendered with class"
    };
    const d: uPlot.AlignedData = [
      mockData[0].slice(start, len),
      mockData[1].slice(start, len)
    ];
    this.setState({ d, options });
    console.log(d);*/
  })

  const [ greeting, setGreeting ] = useState("World");

  useEffect(() => {
    const f = async() => {
      const res = await fetch(import.meta.env.VITE_SERVER_URL + "greet");
      console.log(res);
      const greet = await res.json(); 
      console.log(greet);
      setGreeting(greet.greeting);
    }
    f();
  }, [greeting]);

  return (
    <div className="App">
      <p>Hello {greeting}</p>
      <button onClick={() => {
        socket.emit("hello", "server");
      }}>Click to emit</button>
      <ClassApp></ClassApp>
    </div>
  )
}

export default App
