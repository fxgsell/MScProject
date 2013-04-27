#include <stdio.h>
#include <unistd.h>


#include "robot.hpp"
#include "leg.hpp"
#include "event.hpp"
#include "socket.hpp"

extern Body *robot;
extern int stepCount;

int step() {
  static packet c;

  if (stepCount != 0 || robot->x || robot->y || robot->turn) {
    if (stepCount == 0) {
      printf("Stepping %d %d %d\n", robot->x, robot->y, robot->turn);
      c.x = robot->x;
      c.y = robot->y;
      c.turn = robot->turn;
      for (int i = 0; i < Body::LEGS; i += 2) {
        robot->legs[i]->setShoulder(c.turn * 10);
        robot->legs[i]->updateCoord(0, -50, 0);  //UP
      }
    }
    else if (stepCount == 1) {
      for (int i = 0; i < Body::LEGS; i += 2) {
        robot->legs[i]->updateCoord(-5 * c.y, 0, -5 * c.x);  //forward
      }
    }
    else if (stepCount == 2) {
      for (int i = 0; i < Body::LEGS; i += 2) {
        robot->legs[i]->updateCoord(0, 50, 0);  //DOWN
      }
    }
    else if (stepCount == 3) {
      for (int i = 1; i < Body::LEGS; i += 2) {
        robot->legs[i]->setShoulder(c.turn * 10);
        robot->legs[i]->updateCoord(0, -50, 0);  //UP
      }
    }
    else if (stepCount == 4) {
      for (int i = 1; i < Body::LEGS; i += 2) {
        robot->legs[i]->updateCoord(-5 * c.y, 0, -5 * c.x);  //forward
      }
    }
    else if (stepCount == 5) {
      for (int i = 1; i < Body::LEGS; i += 2) {
        robot->legs[i]->updateCoord(0, 50, 0);  //DOWN
      }
    }
    else if (stepCount == 6) {
      for (int i = 0; i < Body::LEGS; i++) {
        robot->legs[i]->setShoulder(0);
        robot->legs[i]->updateCoord(5 * c.y, 0, 5 * c.x);  //backward
      }
    }
    stepCount++;
    stepCount = stepCount % 7;
    printf("%d\n", stepCount);
    return robot->commit();
  }
  return 0;
}

