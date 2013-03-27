#include <list>

#include "action.hpp"
#include "event.hpp"
#include "robot.hpp"

std::list<Event *> *standDown() {
  std::list<Event *> *standDown = new std::list<Event *>;

  standDown->push_back(new ELegSet(*robot, Leg::ALL, 0, 300, 300));
  standDown->push_back(new ESleep(100000));
  return standDown;
}

std::list<Event *> *walk(int steps) {
  std::list<Event *> *walk = new std::list<Event *>;

  for (; steps; steps--) {
    walk->push_back(new ELegUp(*robot, Leg::A, 400));
    walk->push_back(new ELegForward(*robot, Leg::A, 400));
    walk->push_back(new ESleep(100000));
    walk->push_back(new ELegBackward(*robot, Leg::B, 400));
    walk->push_back(new ESleep(100000));
    walk->push_back(new ELegDown(*robot, Leg::A, 400));
    walk->push_back(new ESleep(100000));
    walk->push_back(new ELegUp(*robot, Leg::B, 400));
    walk->push_back(new ELegForward(*robot, Leg::B, 400));
    walk->push_back(new ESleep(100000));
    walk->push_back(new ELegBackward(*robot, Leg::A, 400));
    walk->push_back(new ESleep(100000));
    walk->push_back(new ELegDown(*robot, Leg::B, 400));
  }
  return walk;
}

std::list<Event *> *hello(Leg::LegId) {
  std::list<Event *> *hello = new std::list<Event *>;

  /*
  **  Need backup event
  */


  return hello;
}

std::list<Event *> *standUp() {
  std::list<Event *> *standUp = new std::list<Event *>;

  standUp->push_back(new ELegSet(*robot, Leg::A, 0, 0, 0));
  standUp->push_back(new ELegSet(*robot, Leg::B, 0, 500, 500));
  standUp->push_back(new ESleep(100000));
  standUp->push_back(new ELegSet(*robot, Leg::A, 0, 500, 500));
  standUp->push_back(new ELegSet(*robot, Leg::B, 0, 0, 0));
  standUp->push_back(new ESleep(100000));
  standUp->push_back(new ELegSet(*robot, Leg::A, 0, -300, -300));
  standUp->push_back(new ELegSet(*robot, Leg::B, 0, 0, 0));
  standUp->push_back(new ESleep(100000));
  standUp->push_back(new ELegSet(*robot, Leg::A, 0, -300, -300));
  standUp->push_back(new ELegSet(*robot, Leg::B, 0, -300, -300));
  standUp->push_back(new ESleep(100000));
  return standUp;
}


