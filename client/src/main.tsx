import React, { createContext } from 'react'
import ReactDOM from 'react-dom/client'
import App from './App'
import './index.css'

import { SupabaseClient, createClient } from '@supabase/supabase-js'
import App2 from './App2'
export const SB_CONTEXT = createContext<SupabaseClient | null>(null);

ReactDOM.createRoot(document.getElementById('root') as HTMLElement).render(
  //<React.StrictMode>
  <SB_CONTEXT.Provider value={createClient(import.meta.env.VITE_SB_URL, import.meta.env.VITE_SB_KEY)}>
    <App2 />
  </SB_CONTEXT.Provider>
  //</React.StrictMode>,
)
