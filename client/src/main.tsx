import React, { createContext } from 'react'
import ReactDOM from 'react-dom/client'
import App from './App'
import { io, Socket } from 'socket.io-client' 
import './index.css'

export const SocketContext = createContext<Socket | null>(null);

ReactDOM.createRoot(document.getElementById('root') as HTMLElement).render(
  //<React.StrictMode>
    <SocketContext.Provider value={io(import.meta.env.VITE_SERVER_URL)}>
      <App />
    </SocketContext.Provider>
  //</React.StrictMode>,
)
