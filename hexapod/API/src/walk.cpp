#include <stdio.h>
#include <unistd.h>


#include "robot.hpp"
#include "leg.hpp"
#include "event.hpp"
#include "socket.hpp"

extern Body *robot;
extern int stepCount;

int step() {
  static int step = 0;
  static packet current;

  if (step != 0 || robot->x || robot->y || robot->turn) {
    if (step == 0) {
      printf("Stepping %d %d %d\n", robot->x, robot->y, robot->turn);
      current.x = robot->x;
      current.y = robot->y;
      current.turn = robot->turn;
      for (int i = 0; i < Body::LEGS; i += 2) {
        robot->legs[i]->setShoulder(current.turn * 10);
        robot->legs[i]->updateCoord(0, -50, 0);  //UP
      }
    }
    else if (stepCount == 1) {
      for (int i = 0; i < Body::LEGS; i += 2) {
        robot->legs[i]->updateCoord(-5 * current.y, 0, -5 * current.x);  //forward
      }
    }
    else if (stepCount == 2) {
      for (int i = 0; i < Body::LEGS; i += 2) {
        robot->legs[i]->updateCoord(0, 50, 0);  //DOWN
      }
    }
    else if (stepCount == 3) {
      for (int i = 1; i < Body::LEGS; i += 2) {
        robot->legs[i]->setShoulder(current.turn * 10);
        robot->legs[i]->updateCoord(0, -50, 0);  //UP
      }
    }
    else if (stepCount == 4) {
      for (int i = 1; i < Body::LEGS; i += 2) {
        robot->legs[i]->updateCoord(-5 * current.y, 0, 5 * current.x);  //forward
      }
    }
    else if (stepCount == 5) {
      for (int i = 1; i < Body::LEGS; i += 2) {
        robot->legs[i]->updateCoord(0, 50, 0);  //DOWN
      }
    }
    else if (stepCount == 6) {
      for (int i = 0; i < Body::LEGS; i += 1) {
        robot->legs[i]->angle = 0;
        robot->legs[i]->updateCoord(5 * current.y, 0, 5 * current.x);  //backward
      }
    }
    step++;
    stepCount %= 7;
    return robot->commit();
  }
  return 0;
}

