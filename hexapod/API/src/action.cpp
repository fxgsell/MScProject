#include <list>

#include "action.hpp"
#include "event.hpp"
#include "robot.hpp"

std::list<Event *> *reverseStandUp() {
  std::list<Event *> *reverseStandUp = new std::list<Event *>;
  reverseStandUp->push_back(new ELegSet(*robot, Leg::ALL, 0, 1000, 0));   // REVERSE GROUND

 return reverseStandUp;

}

std::list<Event *> *flipOver() {
  std::list<Event *> *flipOver = new std::list<Event *>;
  std::list<Event *> *a;

// STANDUP ON BACK
//  a = reverseStandUp();
//  flipOver->insert(flipOver->end(), a->begin(), a->end());
//  flipOver->push_back(new ESleep(10000000));



// ALL GOOD FROM HERE
// MAULAL SET IN RIGHT POSITION
flipOver->push_back(new ELegSet(*robot, Leg::ALL, 0, 1000, -1000));   // REVERSE GROUND
flipOver->push_back(new ESleep(5000000));

// BACK TO SIDE
{ // SIDE STAND
  // LM IN
  // LF IN
  flipOver->push_back(new ELegSet(*robot, Leg::MR, 0, -1000, -700));
  flipOver->push_back(new ELegSet(*robot, Leg::FR, 0, -1000, -700));
  flipOver->push_back(new ESleep(100000));

  // MR INTERNEDIATE
  // BL INTERNEDIATE
  flipOver->push_back(new ELegSet(*robot, Leg::ML, 0, 1000, -1000));
  flipOver->push_back(new ELegSet(*robot, Leg::BR, 0, 1000, -1000));
  flipOver->push_back(new ESleep(100000));

  // FR OUT
  // BR OUT
  flipOver->push_back(new ELegSet(*robot, Leg::FL, 0, 900, -900));
  flipOver->push_back(new ELegSet(*robot, Leg::BL, 0, 900, -900));
  flipOver->push_back(new ESleep(100000));
}

//SIDE TO NORMAL
{ //PUSH
  flipOver->push_back(new ELegSet(*robot, Leg::FL, 0, 0, 0));
  flipOver->push_back(new ELegSet(*robot, Leg::BL, 0, 0, 0));
  flipOver->push_back(new ESleep(100000));

}

//  STANDUP
  a = standUp();
  flipOver->insert(flipOver->end(), a->begin(), a->end());
  flipOver->push_back(new ESleep(100000));

  return flipOver;
}

std::list<Event *> *standDown() {
  std::list<Event *> *standDown = new std::list<Event *>;

  standDown->push_back(new ELegSet(*robot, Leg::ALL, 0, 300, 300));
  standDown->push_back(new ESleep(100000));
  return standDown;
}

std::list<Event *> *positionWalk() {
  std::list<Event *> *positionWalk = new std::list<Event *>;

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


std::list<Event *> *walk(int steps) {
  std::list<Event *> *walk = new std::list<Event *>;

  for (; steps; steps--) {
    walk->push_back(new ELegUp(*robot, Leg::A, 5));
    walk->push_back(new ELegForward(*robot, Leg::A, 200));
    walk->push_back(new ESleep(100000));
    walk->push_back(new ELegBackward(*robot, Leg::B, 200));
    walk->push_back(new ESleep(100000));
    walk->push_back(new ELegDown(*robot, Leg::A, 5));
    walk->push_back(new ESleep(100000));
    walk->push_back(new ELegUp(*robot, Leg::B, 5));
    walk->push_back(new ELegForward(*robot, Leg::B, 200));
    walk->push_back(new ESleep(100000));
    walk->push_back(new ELegBackward(*robot, Leg::A, 200));
    walk->push_back(new ESleep(100000));
    walk->push_back(new ELegDown(*robot, Leg::B, 5));
  }
  return walk;
}

std::list<Event *> *hello(Leg::LegId l) {
  std::list<Event *> *hello = new std::list<Event *>;

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
std::list<Event *> *centerLegs() {
  std::list<Event *> *centerLegs = new std::list<Event *>;
  centerLegs->push_back(new ELegUp(*robot, Leg::A, 100));
  centerLegs->push_back(new ELegCenter(*robot, Leg::A));
  centerLegs->push_back(new ELegDown(*robot, Leg::A, 100));
  centerLegs->push_back(new ESleep(100000));
  centerLegs->push_back(new ELegUp(*robot, Leg::B, 100));
  centerLegs->push_back(new ELegCenter(*robot, Leg::B));
  centerLegs->push_back(new ELegDown(*robot, Leg::B, 100));
  return centerLegs;
}

std::list<Event *> *setLegVLevel(Leg::LegId id, int level) {
  std::list<Event *> *setLegVLevel = new std::list<Event *>;
  std::list<Event *> *a;

  //// Center the legs
  //a = centerLegs();
  //standUp->insert(standUp->end(), a->begin(), a->end());
  //standUp->push_back(new ESleep(100000));

  setLegVLevel->push_back(new ELegVLevel(*robot, id, level));
 
  return setLegVLevel;
}


std::list<Event *> *standUp() {
  std::list<Event *> *standUp = new std::list<Event *>;
  std::list<Event *> *a;

  //// Center the legs
  //a = centerLegs();
  //standUp->insert(standUp->end(), a->begin(), a->end());
  //standUp->push_back(new ESleep(100000));

  standUp->push_back(new ESleep(100000));

  for (int i = 0; i < 20; i++) {
    standUp->push_back(new ELegDown(*robot, Leg::A, 200)); 
    standUp->push_back(new ESleep(100000));
    standUp->push_back(new ELegDown(*robot, Leg::B, 200)); 
    standUp->push_back(new ESleep(100000));
  }
 
  return standUp;
}


