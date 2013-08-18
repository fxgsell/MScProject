# Setup Development Environment on Linux

## Install

Tested on Fedora 19, should be similar in other distrib.

First install dependency: (second line only for the viewer)
```
yum install make gcc-c++ SDL-devel SDL_image-devel
yum install npm
```

And node.js required modules (only for the viewer)
```
npm install string  
npm install socket.io
```

## Run
### Viewer
The robot must be running already. And you browser must support websockets and webGL.
```
cd HexapodViewer
node server.js _ip_of_robot_
```
Once running just load the spider.html file in your browser


### Remote Control
A joystick must be plugged in the computer.
```
cd RemoteControl
make
./control _ip_of_robot_
```
