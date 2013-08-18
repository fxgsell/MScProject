#pragma once

struct packet {
  int id;
  int flags;
  int x;           // Axis 0
  int y;           // Axis 1
  int turn;        // Axis 2
  int height;      // Axis 3
};

#define B01 1
#define B02 1 << 1
#define B03 1 << 2
#define B04 1 << 3
#define B05 1 << 4  // CLEAR EVENTS
#define B06 1 << 5  // RESET POSITION
#define B11 1 << 6  // STANDUP
#define B12 1 << 7  // SITDOWN

#define BUFLEN 512
#define NPACK 10
#define PORT 9930

#define SRV_IP "999.999.999.999"

