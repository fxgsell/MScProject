#include <list>

#include "action.hpp"
#include "event.hpp"
#include "robot.hpp"

std::list<Event *> *standDown() {
  std::list<Event *> *standDown = new std::list<Event *>;

  standDown->push_back(new ESetAllLeg(*robot, 0, 300, 300));
  standDown->push_back(new ESleep(100000));
  return standDown;
}

std::list<Event *> *standUp() {
  std::list<Event *> *standUp = new std::list<Event *>;

  standUp->push_back(new ESetALeg(*robot, 0, 0, 0));
  standUp->push_back(new ESetBLeg(*robot, 0, 500, 500));
  standUp->push_back(new ESleep(100000));
  standUp->push_back(new ESetALeg(*robot, 0, 500, 500));
  standUp->push_back(new ESetBLeg(*robot, 0, 0, 0));
  standUp->push_back(new ESleep(100000));
  standUp->push_back(new ESetALeg(*robot, 0, -300, -300));
  standUp->push_back(new ESetBLeg(*robot, 0, 0, 0));
  standUp->push_back(new ESleep(100000));
  standUp->push_back(new ESetALeg(*robot, 0, -300, -300));
  standUp->push_back(new ESetBLeg(*robot, 0, -300, -300));
  standUp->push_back(new ESleep(100000));
  return standUp;
}


