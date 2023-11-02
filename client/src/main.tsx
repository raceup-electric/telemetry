import React, { createContext } from 'react'
import ReactDOM from 'react-dom/client'
import './index.css'

import { SupabaseClient, createClient } from '@supabase/supabase-js'
import App from './App'
export const SB_CONTEXT = createContext<SupabaseClient | null>(null);

ReactDOM.createRoot(document.getElementById('root') as HTMLElement).render(
  //<React.StrictMode>
  <SB_CONTEXT.Provider value={createClient(import.meta.env.VITE_SB_URL, import.meta.env.VITE_SB_KEY)}>
    <App />
  </SB_CONTEXT.Provider>
  //</React.StrictMode>,
)
