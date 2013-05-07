#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "robot.hpp"
#include "leg.hpp"
#include "event.hpp"
#include "socket.hpp"

extern Body *robot;

const double TURNCOEF = 2.0;
const double VERTCOEF = 1;
const double HORICOEF = 5.0;

#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)

enum LegMoves {UP = 0, FWD = 1, DOWN = 2, BWD = 3};

static int stepCount = 0;

int startwalk(packet &c) {

  if (stepCount == 0) {
    for (int i = 0; i < Body::LEGS; i += 2) {
      c.height = robot->height;
      c.height = MAX(c.height, 10);
      c.height = MIN(c.height, 21);
      robot->legs[i]->up(VERTCOEF * c.height);
    }
  }
  else if (stepCount == 1) {
    for (int i = 0; i < Body::LEGS; i += 2) {
      robot->legs[i]->forward((HORICOEF * c.x) / 2);  //FORWARD HALF
    }
  }
  else if (stepCount == 2) {
    for (int i = 0; i < Body::LEGS; i += 2) {
      robot->legs[i]->down(VERTCOEF * c.height);
    }
  }
  else if (stepCount == 3) {
    for (int i = 0; i < Body::LEGS; i += 2) { //GROUP A
      robot->legs[i]->backward(HORICOEF * c.x);  //BACKWARD COMPLETE
    }
    for (int i = 1; i < Body::LEGS; i += 2) { //GROUP B
      c.height = robot->height;
      c.height = MAX(c.height, 10);
      c.height = MIN(c.height, 21);
      robot->legs[i]->up(VERTCOEF * c.height);
    }
  }
  else if (stepCount == 4) {
    for (int i = 1; i < Body::LEGS; i += 2) {
      robot->legs[i]->forward((HORICOEF * c.x) / 2);  //FORWARD HALF
    }
  }
  else if (stepCount == 5) {
    for (int i = 1; i < Body::LEGS; i += 2) {
      robot->legs[i]->down(VERTCOEF * c.height);
    }
    robot->gaitStatus = Body::WALKING;
  }
  stepCount++;
  stepCount %= 6;
  return robot->commit();
}

int stopwalk(packet &c) {

  if (stepCount == 0) {
    for (int i = 0; i < Body::LEGS; i += 2) {
      c.height = robot->height;
      c.height = MAX(c.height, 10);
      c.height = MIN(c.height, 21);
      robot->legs[i]->up(VERTCOEF * c.height);
    }
    for (int i = 1; i < Body::LEGS; i += 2) { //GROUP B
      robot->legs[i]->backward((HORICOEF * c.x)/2);
    }
  }
  else if (stepCount == 1) {
    for (int i = 0; i < Body::LEGS; i += 2) {
      robot->legs[i]->forward((HORICOEF * c.x)/2);
    }
  }
  else if (stepCount == 2) {
    for (int i = 0; i < Body::LEGS; i += 2) {
      robot->legs[i]->down(VERTCOEF * c.height);
    }
    stepCount = -1;
  }
  stepCount++;
  return robot->commit();
}


     // robot->legs[i]->setShoulder(c.turn * TURNCOEF);

int step(packet &c) {

  if (stepCount == 0) {
    for (int i = 0; i < Body::LEGS; i += 2) { //GROUP A 
      c.height = robot->height;
      c.height = MAX(c.height, 10);
      c.height = MIN(c.height, 21);
      robot->legs[i]->up(VERTCOEF * c.height);
    }
    for (int i = 1; i < Body::LEGS; i += 2) { //GROUP B 
      robot->legs[i]->backward(HORICOEF * c.x);
    }
  }
  else if (stepCount == 1) {
    for (int i = 0; i < Body::LEGS; i += 2) {
      robot->legs[i]->forward(HORICOEF * c.x);
    }
  }
  else if (stepCount == 2) {
    for (int i = 0; i < Body::LEGS; i += 2) {
      robot->legs[i]->down(VERTCOEF * c.height);
    }
  }
  else if (stepCount == 3) {
    for (int i = 0; i < Body::LEGS; i += 2) { //GROUP A
      robot->legs[i]->backward(HORICOEF * c.x);
    }
    for (int i = 1; i < Body::LEGS; i += 2) { //GROUP B
      c.height = robot->height;
      c.height = MAX(c.height, 10);
      c.height = MIN(c.height, 21);
      robot->legs[i]->up(VERTCOEF * c.height);
    }
  }
  else if (stepCount == 4) {
    for (int i = 1; i < Body::LEGS; i += 2) {
      robot->legs[i]->forward(HORICOEF * c.x);
    }
  }
  else if (stepCount == 5) {
    for (int i = 1; i < Body::LEGS; i += 2) {
      robot->legs[i]->down(VERTCOEF * c.height);
    }
    robot->gaitStatus = Body::WALKING;
  }
  stepCount++;
  stepCount %= 6;
  return robot->commit();
}

int walk() {
  static packet c = {0, 0, -10, 0, 0, 21};
  static int i = 0;

  int stop = 6*24;
  while (i < stop - 3) {
    i++;

    if  (i == stop - 6)
      robot->gaitStatus = Body::STOPGAIT;
    if (stepCount == 0 && c.x == 0 && c.y == 0 &&
	robot->gaitStatus == Body::WALKING)
      robot->gaitStatus = Body::STOPGAIT;

    if (robot->gaitStatus == Body::STARTGAIT)
      return (startwalk(c));
    else if (robot->gaitStatus == Body::WALKING) {
      return (step(c)); }
    else if (robot->gaitStatus == Body::STOPGAIT)
      return (stopwalk(c));
  }
  exit(0);
  return 0;
}

