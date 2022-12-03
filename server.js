const express = require('express');
const socketIO = require('socket.io');
const app = express();
const http = require('http');
const path = require("path");
const server = http.createServer(app);
const io = socketIO(server);

const port = 3000;
app.set('port', port);

app.use("/", express.static(__dirname + '/dist'));
app.get('/', function (request, response) {
    response.sendFile(path.join(__dirname, '/dist/index.html'));
});

server.listen(port, function () {
    console.log('Starting server on port ' + port);
});

let MILLIS = 5000;
io.on("connection", (socket) => {
    setInterval(function () {
        //data collection
        let data = {
            "temperature": {
                "motors": {
                    "fl": Math.random(), //front left
                    "fr": Math.random(), //front right
                    "rl": Math.random(), //rear left
                    "rr": Math.random() //rear right
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
                }
            },
            "voltage": {
                "hv": {
                    "high": Math.random(),
                    "low": Math.random(),
                    "avg": Math.random()
                }
            }
        }
        socket.broadcast.emit("data", data);
    }, MILLIS)
});