#!/bin/sh

GRAFANA_URL="http://dashboard:3000"
ADMIN_USER="admin"
ADMIN_PASSWORD="a"

rm -rf /run/shared/service-account-token.txt

# Install necessary tools
apk add --no-cache curl jq netcat-openbsd tcpdump


# Wait for Grafana port to be open
echo "Waiting for Grafana port 3000..."
while ! nc -z dashboard 3000; do
  sleep 1
done

# Wait for Grafana API to be ready
echo "Waiting for Grafana API to be ready..."
until curl -s -u admin:a http://dashboard:3000/api/health | jq -e '.database == "ok"' >/dev/null; do
  echo "Still waiting..."
  sleep 2
done

echo "Grafana is ready!"

# Now create the service account and token
GRAFANA_USER=admin
GRAFANA_PASS=a

# Create service account
SERVICE_ACCOUNT_JSON=$(curl -s -u "$GRAFANA_USER:$GRAFANA_PASS" \
  -X POST http://dashboard:3000/api/serviceaccounts \
  -H "Content-Type: application/json" \
  -d '{"name":"iot-service","role":"Admin"}')

SERVICE_ACCOUNT_ID=$(echo "$SERVICE_ACCOUNT_JSON" | jq -r '.id')

# Create token
TOKEN_JSON=$(curl -s -u "$GRAFANA_USER:$GRAFANA_PASS" \
  -X POST http://dashboard:3000/api/serviceaccounts/${SERVICE_ACCOUNT_ID}/tokens \
  -H "Content-Type: application/json" \
  -d '{"name": "iot-service-token", "secondsToLive": 0}')

echo "Created token:"
echo "$TOKEN_JSON" | jq -r '.key' > /run/shared/service-account-token.txt
cat /run/shared/service-account-token.txt


# List of users to create

FILE="account.csv"

curl -sS -L -o $FILE "https://docs.google.com/spreadsheets/d/1uOm5v2PGaIuHu4JrMOmmV8THiIVy6Vdb9NubWfWz5HY/export?format=csv"

echo >> $FILE
# 2) Skip header, then split on commas:
#    _ _  name         email                      user       rest
tail -n +2 "$FILE" | while IFS=, read name email user rest; do

  # If anything spilled into $rest, that's >5 columns → error out
  if [ -n "$rest" ]; then
    echo "Error: unexpected extra field(s): '$rest'"
  else
    clean_name="${name// /}"
    first2="${clean_name:0:2}"
    last3="${clean_name: -3}"
    passwd="${first2}${last3}"

    echo
    echo "Creating user: $user"

    # Otherwise process your three fields:
    curl -s -X POST "$GRAFANA_URL/api/admin/users" \
      -u "$ADMIN_USER:$ADMIN_PASSWORD" \
      -H "Content-Type: application/json" \
      -d @- <<EOF
    {
      "name": "$name",
      "email": "$email",
      "login": "$user",
      "password": "$passwd"
    }
EOF
  fi
done


