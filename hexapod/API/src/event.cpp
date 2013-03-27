#include <unistd.h>

#include "event.hpp"

//Should be action
int	EStandDown::execute() {
  robot.setAllLeg(0, 300, 300);
  robot.commit();
  return (0);
}

int	ESetALeg::execute() {
  for (int i = 0; i < Body::LEGS; i++) {
    if (robot.legs[i]->group == Leg::A)
      robot.legs[i]->setPosition(s, e, w);
  }
  robot.commit();
  return (0);
}

/*
** Backward A, B
*/
int	EBackwardALeg::execute() {
  for (int i = 0; i < Body::LEGS; i++) {
    if (robot.legs[i]->group == Leg::A)
      robot.legs[i]->backward(range);
  }
  robot.commit();
  return (0);
}

int	EBackwardBLeg::execute() {
  for (int i = 0; i < Body::LEGS; i++) {
    if (robot.legs[i]->group == Leg::B)
      robot.legs[i]->backward(range);
  }
  robot.commit();
  return (0);
}


/*
** Forward A, B
*/
int	EForwardALeg::execute() {
  for (int i = 0; i < Body::LEGS; i++) {
    if (robot.legs[i]->group == Leg::A)
      robot.legs[i]->forward(range);
  }
  robot.commit();
  return (0);
}

int	EForwardBLeg::execute() {
  for (int i = 0; i < Body::LEGS; i++) {
    if (robot.legs[i]->group == Leg::B)
      robot.legs[i]->forward(range);
  }
  robot.commit();
  return (0);
}

/*
** Down A, B
*/
int	EDownALeg::execute() {
  for (int i = 0; i < Body::LEGS; i++) {
    if (robot.legs[i]->group == Leg::A)
      robot.legs[i]->down(range);
  }
  robot.commit();
  return (0);
}

int	EDownBLeg::execute() {
  for (int i = 0; i < Body::LEGS; i++) {
    if (robot.legs[i]->group == Leg::B)
      robot.legs[i]->down(range);
  }
  robot.commit();
  return (0);
}

/*
** Up A, B
*/
int	EUpALeg::execute() {
  for (int i = 0; i < Body::LEGS; i++) {
    if (robot.legs[i]->group == Leg::A)
      robot.legs[i]->up(range);
  }
  robot.commit();
  return (0);
}

int	EUpBLeg::execute() {
  for (int i = 0; i < Body::LEGS; i++) {
    if (robot.legs[i]->group == Leg::B)
      robot.legs[i]->up(range);
  }
  robot.commit();
  return (0);
}

/*
** Set A, B, All
*/
int	ESetAllLeg::execute() {
  for (int i = 0; i < Body::LEGS; i++)
    robot.legs[i]->setPosition(s, e, w);
  robot.commit();
  return (0);
}

int	ESetBLeg::execute() {
  for (int i = 0; i < Body::LEGS; i++) {
    if (robot.legs[i]->group == Leg::B)
      robot.legs[i]->setPosition(s, e, w);
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

