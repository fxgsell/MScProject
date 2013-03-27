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

std::list<Event *> *walk(int steps) {
  std::list<Event *> *walk = new std::list<Event *>;

  for (; steps; steps--) {
    walk->push_back(new EUpALeg(*robot, 400));
    walk->push_back(new EForwardALeg(*robot, 400));
    walk->push_back(new ESleep(100000));
    walk->push_back(new EBackwardBLeg(*robot, 400));
    walk->push_back(new ESleep(100000));
    walk->push_back(new EDownALeg(*robot, 400));
    walk->push_back(new ESleep(100000));
    walk->push_back(new EUpBLeg(*robot, 400));
    walk->push_back(new EForwardBLeg(*robot, 400));
    walk->push_back(new ESleep(100000));
    walk->push_back(new EBackwardALeg(*robot, 400));
    walk->push_back(new ESleep(100000));
    walk->push_back(new EDownBLeg(*robot, 400));
  }
  return walk;
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


