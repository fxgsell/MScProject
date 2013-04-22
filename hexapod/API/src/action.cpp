#include "list.hpp"

#include "action.hpp"
#include "event.hpp"
#include "robot.hpp"


list *positionWalk() {
  list *positionWalk = new list;

  positionWalk->push_back(new ELegUp(*robot, Leg::FRONT, 5));
  positionWalk->push_back(new ESleep(100000));
  positionWalk->push_back(new ELegForward(*robot, Leg::FRONT, 500));
  positionWalk->push_back(new ESleep(100000));
  positionWalk->push_back(new ELegDown(*robot, Leg::FRONT, 5));
  positionWalk->push_back(new ESleep(100000));


  positionWalk->push_back(new ELegUp(*robot, Leg::MIDLE, 5));
  positionWalk->push_back(new ESleep(100000));
  positionWalk->push_back(new ELegForward(*robot, Leg::MIDLE, 0));
  positionWalk->push_back(new ESleep(100000));
  positionWalk->push_back(new ELegDown(*robot, Leg::MIDLE, 5));
  positionWalk->push_back(new ESleep(100000));


  positionWalk->push_back(new ELegUp(*robot, Leg::BACK, 5));
  positionWalk->push_back(new ESleep(100000));
  positionWalk->push_back(new ELegForward(*robot, Leg::BACK, -500));
  positionWalk->push_back(new ESleep(100000));
  positionWalk->push_back(new ELegDown(*robot, Leg::BACK, 5));
  positionWalk->push_back(new ESleep(100000));

  return positionWalk;
}

#include <stdio.h>

list *walk(int steps) {
  list *walk = new list;

  for (; steps; steps--) {
    walk->push_back(new ELegDown(*robot, Leg::A, 50));
    walk->push_back(new ELegForward(*robot, Leg::A, 40));
    walk->push_back(new ESleep(100000));
    walk->push_back(new ELegBackward(*robot, Leg::B, 40));
    walk->push_back(new ESleep(100000));
    walk->push_back(new ELegUp(*robot, Leg::A, 50));
    walk->push_back(new ESleep(100000));
    walk->push_back(new ELegDown(*robot, Leg::B, 50));
    walk->push_back(new ELegForward(*robot, Leg::B, 40));
    walk->push_back(new ESleep(100000));
    walk->push_back(new ELegBackward(*robot, Leg::A, 40));
    walk->push_back(new ESleep(100000));
    walk->push_back(new ELegUp(*robot, Leg::B, 50));
    walk->push_back(new ESleep(100000));
  }

  return walk;
}

list *hello(Leg::LegId l) {
  list *hello = new list;

  hello->push_back(new ELegSave(*robot, l));
  for (int i = 0; i < 4; i++) {
    hello->push_back(new ELegSet(*robot, l, -200, 1000, -600));
    hello->push_back(new ESleep(100000));
    hello->push_back(new ELegSet(*robot, l, 200, 1000, -400));
    hello->push_back(new ESleep(100000));
    hello->push_back(new ELegSet(*robot, l, -200, 1000, -600));
    hello->push_back(new ESleep(100000));
  }
  hello->push_back(new ESleep(100000));
  hello->push_back(new ELegRestore(*robot, l));
  hello->push_back(new ESleep(100000));

  return hello;
}

/*
** Align the legs
*/
list *centerLegs() {
  list *centerLegs = new list;
  centerLegs->push_back(new ELegUp(*robot, Leg::A, 100));
  centerLegs->push_back(new ELegCenter(*robot, Leg::A));
  centerLegs->push_back(new ELegDown(*robot, Leg::A, 100));
  centerLegs->push_back(new ESleep(100000));
  centerLegs->push_back(new ELegUp(*robot, Leg::B, 100));
  centerLegs->push_back(new ELegCenter(*robot, Leg::B));
  centerLegs->push_back(new ELegDown(*robot, Leg::B, 100));
  return centerLegs;
}

list *setLegVLevel(Leg::LegId id, int level) {
  list *setLegVLevel = new list;

  //// Center the legs
  //a = centerLegs();
  //standUp->insert(standUp->end(), a->begin(), a->end());
  //standUp->push_back(new ESleep(100000));

  setLegVLevel->push_back(new ELegVLevel(*robot, id, level));
 
  return setLegVLevel;
}


list *standUp() {
  list *standUp = new list;

  for (int i = 0; i < 20; i++) {
    standUp->push_back(new ELegSetCoord(*robot, Leg::ALL, 150, 40 + i*5, 0));
  }
 
  return standUp;
}

list *standDown() {
  list *standDown = new list;

  for (int i = 20; i > 0; i--) {
    standDown->push_back(new ELegSetCoord(*robot, Leg::ALL, 150, 40 + i*5, 0));
  }
  return standDown;
}

