while [ ! -f /run/shared/service-account-token.txt ]; do
  echo "Waiting for grafana-init to finish..."
  sleep 1
done

GRAFANA_TOKEN=$(cat /run/shared/service-account-token.txt)
export GRAFANA_TOKEN

echo $GRAFANA_TOKEN
exec telegraf

