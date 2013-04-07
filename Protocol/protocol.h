#pragma once

struct packet {
        int      flags;
        long int id;
        int      orientation; // Axis 3
        int      x;           // Axis 0
        int      y;           // Axis 1
        int      turn;        // Axis 2
        int      speed;       // Axis 3
};


#define BUFLEN 512
#define NPACK 10
#define PORT 9930

#define SRV_IP "999.999.999.999"

