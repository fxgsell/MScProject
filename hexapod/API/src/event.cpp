#include "event.hpp"

int	EStandDown::execute() {
  robot.setAllLeg(0, 300, 300);
  robot.commit();
  return (0);
}

int	ESetALeg::execute() {
  for (std::list<Leg *>::iterator it=robot.legs.begin(); it != robot.legs.end(); ++it) {
    if ((*it)->group == Leg::A)
    (*it)->setPosition(s, e, w);
  }
  robot.commit();
  return (0);
}

int	ESetBLeg::execute() {
  for (std::list<Leg *>::iterator it=robot.legs.begin(); it != robot.legs.end(); ++it) {
    if ((*it)->group == Leg::B)
    (*it)->setPosition(s, e, w);
  }
  robot.commit();
  return (0);
}

int	ESleep::execute() {
  usleep(time);
  return (0);
}
