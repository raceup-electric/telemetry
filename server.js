const express = require('express');
const socketIO = require('socket.io');
const app = express();
const http = require('http');
const path = require("path");
const server = http.createServer(app);
const io = socketIO(server);

const port = 8080;
app.set('port', port);

app.use("/", express.static(__dirname + '/dist'));
app.get('/', function (request, response) {
    response.sendFile(path.join(__dirname, '/dist/index.html'));
});

server.listen(port, function () {
    console.log('Starting server on port ' + port);
});

let MILLIS = 100;
io.on("connection", (socket) => {
    setInterval(function () {
        //data collection
        let data = {
            "temperature": {
                "motors": {
                    "fl": Math.random(), 
                    "fr": Math.random(), 
                    "rl": Math.random(), 
                    "rr": Math.random() 
                },
                "igbt": {
                    "fl": Math.random(), 
                    "fr": Math.random(), 
                    "rl": Math.random(), 
                    "rr": Math.random() 
                },
                "inverter": {
                    "fl": Math.random(), 
                    "fr": Math.random(), 
                    "rl": Math.random(), 
                    "rr": Math.random() 
                },
                "cooling": {
                    "preRad": Math.random(),
                    "preMot": Math.random(),
                    "preCold": Math.random(),
                    "postMot": Math.random(),
                    "postCold": Math.random()
                },
                "hv": {
                    "high": Math.random(),
                    "low": Math.random(),
                    "avg": Math.random()
                },
                "lv": {
                    "high": Math.random()
                }
            },
            "voltage": {
                "hv": {
                    "high": Math.random(),
                    "low": Math.random(),
                    "avg": Math.random(),
                },
                "lv": {
                    "total": Math.random(),
                    "low": Math.random()
                },
            },
            "car": {
                "velocity": {
                    "avfl": Math.random() * 250,
                    "avfr": Math.random() * 250,
                    "avrl": Math.random() * 250,
                    "avrr": Math.random() * 250,
                    "av": Math.random() * 250
                },
                "info": {
                    "throttle": Math.random(),
                    "steeringangle": Math.random(),
                    "brake": Math.random(),
                    "brakepress": Math.random()
                },
                "acceleration": {
                    "x": Math.random(),
                    "y": Math.random(),
                    "z": Math.random()
                },
                "omega": {
                    "x": Math.random(),
                    "y": Math.random(),
                    "z": Math.random()
                }
            }
        }
        io.emit("data", data);
    }, MILLIS)
});