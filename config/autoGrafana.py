import pandas as pd

df = pd.read_csv('./si.CSV', sep=';', names=['cognome', 'nome', 'email'])

df['g_name'] = df['nome'] + " " + df['cognome']


import requests

url = "http://admin:G2dmHaDNA5r5YECg24BZDe46om6BpWzf@204.216.214.158:3000/api/admin/users"

for index, row in df.iterrows():
    data = { 
        "name": row['g_name'], 
        "email": row['email'], 
        "login": str(row['email'])[:-10], 
        "password": str(row['nome']).lower()[:2] + str(row['cognome']).lower()[:2], 
        "OrgId": 1 
    }
    x = requests.post(url, json = data)

#print(df['g_name'])