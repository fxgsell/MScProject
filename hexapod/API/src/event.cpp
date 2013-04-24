#include <unistd.h>

#include "robot.hpp"
#include "event.hpp"



/*
** SetCoor
*/
int	ELegSetCoord::execute() {
  for (int i = 0; i < Body::LEGS; i++) {
    if (robot.legs[i]->id & group)
      robot.legs[i]->setCoord(x, y, z);
  }
  robot.commit();
  return (0);
}

/*
** Set
*/
int	ELegSetAngles::execute() {
  for (int i = 0; i < Body::LEGS; i++) {
    if (robot.legs[i]->id & group)
      robot.legs[i]->setAngles(s, e, w);
  }
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
** Backup
*/
int	ELegSave::execute() {
  for (int i = 0; i < Body::LEGS; i++) {
    if (robot.legs[i]->id & group)
      robot.legs[i]->save();
  }
  robot.commit();
  return (0);
}

int	ELegRestore::execute() {
  for (int i = 0; i < Body::LEGS; i++) {
    if (robot.legs[i]->id & group)
      robot.legs[i]->restore();
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

