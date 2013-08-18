# Protocol Specification

The protocol is based on TCP sockets.
It can support different type of clients. Currently either a viewer or a remote control client.

## Handshake
The type of client is defined by sending a first string after initializing the socket.
`"CLIENT"` for remote control.
`"VIEWER"` to get information about servos positions.

## Client Packets
```
struct packet {
  int id;
  int flags;
  int x;           // Axis 0
  int y;           // Axis 1
  int turn;        // Axis 2
  int height;      // Axis 3
};
```

```
#define B01 1
#define B02 1 << 1
#define B03 1 << 2
#define B04 1 << 3
#define B05 1 << 4  // CLEAR EVENTS
#define B06 1 << 5  // RESET POSITION
#define B11 1 << 6  // STANDUP
#define B12 1 << 7  // SITDOWN
```

## Viewer Packets
Viewer packets consist of a string
First word define the type of command and the number of parameters

### "moveLeg"
Allow to set the position of the servos of a leg

`moveLeg LegId ServoId Angle`

* LegId is between 0 and 6 it's the number of the leg
* ServoId is between 0 and 2 it's the number of the servo in the leg
* Angle is between -90 and 90.
