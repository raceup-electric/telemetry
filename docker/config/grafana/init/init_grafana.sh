#!/bin/sh

rm -rf /run/shared/service-account-token.txt

# Install necessary tools
apk add --no-cache curl jq netcat-openbsd


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

