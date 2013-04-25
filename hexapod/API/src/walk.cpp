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

  if (robot->x || robot->y || robot->turn) {
    if (step < 7)
      printf("Baby steps!\n");
    else if (step % 7 == 0) {
      printf("Stepping %d %d %d\n", robot->x, robot->y, robot->turn);
      current.x = robot->x;
      current.y = robot->y;
      current.turn = robot->turn;
      for (int i = 0; i < Body::LEGS; i += 2)
        robot->legs[i]->up(50);
    }
    else if (step % 7 == 1) {
      for (int i = 0; i < Body::LEGS; i += 2) {
        robot->legs[i]->forward(5 * current.x);
        robot->legs[i]->updateShoulder(5 * current.turn);
      }
    }
    else if (step % 7 == 2) {
      for (int i = 0; i < Body::LEGS; i += 2)
        robot->legs[i]->down(50);
    }
    else if (step % 7 == 3) {
      for (int i = 1; i < Body::LEGS; i += 2)
        robot->legs[i]->up(50);
    }
    else if (step % 7 == 4) {
      for (int i = 1; i < Body::LEGS; i += 2) {
        robot->legs[i]->updateShoulder(5 * -current.turn);
        robot->legs[i]->forward(5 * current.x);
      }
    }
    else if (step % 7 == 5) {
      for (int i = 1; i < Body::LEGS; i += 2)
        robot->legs[i]->down(50);
    }
    else if (step % 7 == 6) {
      for (int i = 0; i < Body::LEGS; i += 1)
        robot->legs[i]->backward(5 * current.x);
    }
    robot->commit();
    step++;
  }
  return step;
}

