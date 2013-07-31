var string = require('string');
var http = require('http');
var io = require('socket.io');
  
var host = "" + process.argv.splice(2);
var PORT = 9930;

//WebSocket
var server = http.createServer(function(request, response) {
    response.writeHead(200, {'Content-Type': 'text/html'});
    response.end('WebSocket server is up and running!');
}).listen(33334);
  
//Socket.IO
var socket = io.listen(server).set('log level', 9);
  
var net = require('net');

//Listeners for intercepting client messages
socket.on('connection', function(client) {
    client.on('message', function(data) {
        console.log('Message from client: ', data);
    });
    client.on('customMessage', function (data) {
        console.log('Custom message from client: ', data);
    });
    client.on('disconnect', function(){
        console.log('Client disconnected');
    });


});


var robot = new net.Socket();
robot.connect(9930, host, function() {
    // Something is weird around here
    console.log('Conected to robot: ' + host + ':' + PORT);
    robot.write('VIEWER');
});

// Add a 'data' event handler for the robot socket
// data is what the server sent to this socket
robot.on('data', function(data) {
    console.log('DATA: ' + data);

    var count = 0;
    var partsArray = string(data).replace('\n', '').split(' ');
    for (i = 0; i < partsArray.length; i++) {
       if (partsArray[i] == "moveLeg")
         count = 0;
       if (count == 3) {
         socket.sockets.emit('moveLeg',
			     Number(partsArray[i - 2]),
                             Number(partsArray[i - 1]),
			     Number(partsArray[i]));
       }
       count++;
    }
});

// Add a 'close' event handler for the robot socket
robot.on('close', function() {
    console.log('Connection to the robot closed');
    robot.destroy();
});


