#include <unistd.h>

#include "robot.hpp"
#include "event.hpp"

//Should be action
int	EStandDown::execute() {
  for (int i = 0; i < Body::LEGS; i++)
    robot.legs[i]->setPosition(0, 300, 300);
  robot.commit();
  return (0);
}

/*
** Set
*/
int	ELegSet::execute() {
  for (int i = 0; i < Body::LEGS; i++) {
    if (robot.legs[i]->id & group)
      robot.legs[i]->setPosition(s, e, w);
  }
  robot.commit();
  return (0);
}

/*
** Backward A, B
*/
int	ELegBackward::execute() {
  for (int i = 0; i < Body::LEGS; i++) {
    if (robot.legs[i]->id & group)
      robot.legs[i]->backward(range);
  }
  robot.commit();
  return (0);
}

/*
** Forward A, B
*/
int	ELegForward::execute() {
  for (int i = 0; i < Body::LEGS; i++) {
    if (robot.legs[i]->id & group)
      robot.legs[i]->forward(range);
  }
  robot.commit();
  return (0);
}

/*
** Center
*/
int	ELegCenter::execute() {
  for (int i = 0; i < Body::LEGS; i++) {
    if (robot.legs[i]->id & group)
      robot.legs[i]->center();
  }
  robot.commit();
  return (0);
}

/*
** Down A, B
*/
int	ELegDown::execute() {
  for (int i = 0; i < Body::LEGS; i++) {
    if (robot.legs[i]->id & group)
      robot.legs[i]->down(range);
  }
  robot.commit();
  return (0);
}

/*
** Up A, B
*/
int	ELegUp::execute() {
  for (int i = 0; i < Body::LEGS; i++) {
    if (robot.legs[i]->id & group)
      robot.legs[i]->up(range);
  }
  robot.commit();
  return (0);
}

/*
** Sleep
*/
int	ESleep::execute() {
  usleep(time);
  return (0);
}

