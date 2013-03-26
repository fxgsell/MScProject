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

int	ESleep::execute() {
  usleep(time);
  return (0);
}

