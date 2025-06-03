# Setup

To create a local instance of the current telemetry (2024/2025) you need to have docker
docker-compose. If you have them run:

```sh
docker compose up
```

This command will create three containers with the following containers on your local machine:

- telegraf:1.30
- grafana/grafana-enterprise
- influxdb:2


# Conf

Everything has been already configured, in particular:

- login influxdb
- bucket influxdb
- connection grafana influxdb
- connection telegraf influxdb

What has not been set yet:

- All the dashboards
- grafana query
- telegraf jsons
- automatic key settings for live data from telegraf to grafana

## Grafana Dashboards

To access live data from telegraf you can use:
- data source : --Grafana--
- Query type: Live Measurements
- channel stream/custom_stream_id

For more details check [telegraf-grafana-live](https://grafana.com/tutorials/stream-metrics-from-telegraf-to-grafana/);

To add a new dashboard you can put the json file in:
```sh
./config/grafana/dashboards
```
As axample there already is a dummy dashboard with real time data from telegraf

# Access

## Influxdb:

```sh
http://localhost:8086
login: admin
passwd: admin
```


## Grafana:

```sh
http://localhost:3000
login: admin
passwd: admin
```

