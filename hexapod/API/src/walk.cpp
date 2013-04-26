#include <stdio.h>
#include <unistd.h>


#include "robot.hpp"
#include "leg.hpp"
#include "event.hpp"
#include "socket.hpp"

extern Body *robot;

int step() {
  static int step = 0;
  static int progress = 0;
  static packet current;

  if (step % 7 != 0 || robot->x || robot->y || robot->turn) {
    printf("loop\n");
    if (step % 7 == 0) {
      printf("Stepping %d %d %d\n", robot->x, robot->y, robot->turn);
      current.x = robot->x;
      current.y = robot->y;
      current.turn = robot->turn;
      for (int i = 0; i < Body::LEGS; i += 2) {
        robot->legs[i]->setShoulder(current.turn * 2);
        robot->legs[i]->updateCoord(0, -50, 0);  //UP
      }
    }
    else if (step % 7 == 1) {
      for (int i = 0; i < Body::LEGS; i += 2) {
        robot->legs[i]->updateCoord(-5 * current.y, 0, -5 * current.x);  //forward
      }
    }
    else if (step % 7 == 2) {
      for (int i = 0; i < Body::LEGS; i += 2) {
        robot->legs[i]->updateCoord(0, 50, 0);  //DOWN
      }
    }
    else if (step % 7 == 3) {
      for (int i = 1; i < Body::LEGS; i += 2) {
        robot->legs[i]->setShoulder(current.turn * 2);
        robot->legs[i]->updateCoord(0, -50, 0);  //UP
      }
    }
    else if (step % 7 == 4) {
      for (int i = 1; i < Body::LEGS; i += 2) {
        robot->legs[i]->updateCoord(-5 * current.y, 0, -5 * current.x);  //forward
      }
    }
    else if (step % 7 == 5) {
      for (int i = 1; i < Body::LEGS; i += 2) {
        robot->legs[i]->updateCoord(0, 50, 0);  //DOWN
      }
    }
    else if (step % 7 == 6) {
      for (int i = 0; i < Body::LEGS; i += 1) {
        robot->legs[i]->angle = 0;
        robot->legs[i]->updateCoord(5 * current.y, 0, 5 * current.x);  //backward
      }
    }
    robot->commit();
    step++;
  }
  return 100000;
}

