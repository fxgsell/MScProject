#pragma once

struct packet {
  int id;
  int flags;
  int x;           // Axis 0
  int y;           // Axis 1
  int turn;        // Axis 2
  int height;      // Axis 3
};

#define B10 1
#define B11 1 << 1
#define B07 1 << 2
#define B06 1 << 3

#define BUFLEN 512
#define NPACK 10
#define PORT 9930

#define SRV_IP "999.999.999.999"

