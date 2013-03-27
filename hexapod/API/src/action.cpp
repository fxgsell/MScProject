#include <list>

#include "action.hpp"
#include "event.hpp"
#include "robot.hpp"

std::list<Event *> *reverseStandUp() {
  std::list<Event *> *reverseStandUp = new std::list<Event *>;
  reverseStandUp->push_back(new ELegSet(*robot, Leg::ALL, 0, 1000, 0));   // REVERSE GROUND

  reverseStandUp->push_back(new ESleep(100000));

  int a = 0, b = 0;
  for (int i = 0; i < 20; i++) {
    reverseStandUp->push_back(new ELegSet(*robot, Leg::A, 0,  1000, i * 50)); 
    reverseStandUp->push_back(new ESleep(100000));
    reverseStandUp->push_back(new ELegSet(*robot, Leg::B, 0,  1000, i * 50)); 
    reverseStandUp->push_back(new ESleep(100000));

    a = i * 50;
    if (a > 1000)
      a = 1000;
  }

//reverseStandUp->push_back(new ELegSet(*robot, Leg::ALL, 0, 1000, -1000));   // REVERSE GROUND

/*
  reverseStandUp->push_back(new ELegSet(*robot, Leg::A  , 0, 1000, 0)); 
  reverseStandUp->push_back(new ESleep(100000));
  reverseStandUp->push_back(new ELegSet(*robot, Leg::B  , 0, 1000, 0)); 
  reverseStandUp->push_back(new ESleep(100000));
  reverseStandUp->push_back(new ELegSet(*robot, Leg::A  , 0, 1000, 0)); 
  reverseStandUp->push_back(new ESleep(100000));
  reverseStandUp->push_back(new ELegSet(*robot, Leg::B  , 0, 1000, 0)); 
  reverseStandUp->push_back(new ESleep(100000));
*/

/*
  reverseStandUp->push_back(new ELegSet(*robot, Leg::B, 0, 1000, -0)); // ELBOW AT MAX
  reverseStandUp->push_back(new ESleep(100000));
  reverseStandUp->push_back(new ELegSet(*robot, Leg::A, 0, 200, -1000)); // WRIST AT MAX
  reverseStandUp->push_back(new ESleep(100000));
*/

/*
  reverseStandUp->push_back(new ELegSet(*robot, Leg::B, 0, 1000, -700)); // ELBOW AT MAX
  reverseStandUp->push_back(new ESleep(100000));
  reverseStandUp->push_back(new ELegSet(*robot, Leg::A, 0, 700, -1000)); // WRIST AT MAX
  reverseStandUp->push_back(new ESleep(100000));

  reverseStandUp->push_back(new ELegSet(*robot, Leg::B, 0, 1000, -800)); // ELBOW AT MAX
  reverseStandUp->push_back(new ESleep(100000));
  reverseStandUp->push_back(new ELegSet(*robot, Leg::A, 0, 800, -1000)); // WRIST AT MAX
  reverseStandUp->push_back(new ESleep(100000));

  reverseStandUp->push_back(new ELegSet(*robot, Leg::B, 0, 1000, -900)); // ELBOW AT MAX
  reverseStandUp->push_back(new ESleep(100000));
  reverseStandUp->push_back(new ELegSet(*robot, Leg::A, 0, 900, -1000)); // WRIST AT MAX
  reverseStandUp->push_back(new ESleep(100000));

  reverseStandUp->push_back(new ELegSet(*robot, Leg::B, 0, 1000, -1000)); // ELBOW AT MAX
  reverseStandUp->push_back(new ESleep(100000));
  reverseStandUp->push_back(new ELegSet(*robot, Leg::A, 0, 1000, -1000)); // WRIST AT MAX
  reverseStandUp->push_back(new ESleep(100000));
*/

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

  standUp->push_back(new ELegSet(*robot, Leg::ALL, 0, 1000, 1000));
  standUp->push_back(new ESleep(100000));
  standUp->push_back(new ELegSet(*robot, Leg::ALL, 0, 0, 500));
  standUp->push_back(new ESleep(100000));

  standUp->push_back(new ELegSet(*robot, Leg::A, 0, 200, 200));
  standUp->push_back(new ESleep(100000));
  standUp->push_back(new ELegSet(*robot, Leg::A, 0, 0, 200));
  standUp->push_back(new ESleep(500000));

  standUp->push_back(new ELegSet(*robot, Leg::B, 0, 200, 200));
  standUp->push_back(new ESleep(100000));
  standUp->push_back(new ELegSet(*robot, Leg::B, 0, -200, 200));
  standUp->push_back(new ESleep(500000));

 return standUp;
  standUp->push_back(new ELegSet(*robot, Leg::A, 0, 600, 1000));
  standUp->push_back(new ESleep(100000));
  standUp->push_back(new ELegSet(*robot, Leg::B, 0, 800, 1200));
  standUp->push_back(new ELegSet(*robot, Leg::B, 0, 600, 1000));
  standUp->push_back(new ESleep(100000));
  standUp->push_back(new ELegSet(*robot, Leg::A, 0, 600, 1200));
  standUp->push_back(new ELegSet(*robot, Leg::A, 0, 400, 1000));
  standUp->push_back(new ESleep(100000));
  standUp->push_back(new ELegSet(*robot, Leg::B, 0, 600, 1000));
  standUp->push_back(new ELegSet(*robot, Leg::B, 0, 400, 800));

  standUp->push_back(new ELegSet(*robot, Leg::ALL, 0, -600, 200));

 return standUp;
  standUp->push_back(new ELegSet(*robot, Leg::A, 0, 800, 1000));
  standUp->push_back(new ELegSet(*robot, Leg::B, 0, 800, 1000));
  standUp->push_back(new ESleep(100000));
  standUp->push_back(new ELegSet(*robot, Leg::A, 0, 800, 1000));
  standUp->push_back(new ELegSet(*robot, Leg::B, 0, 800, 1000));
  standUp->push_back(new ESleep(100000));
  standUp->push_back(new ELegSet(*robot, Leg::A, 0, 800, 1000));
  standUp->push_back(new ELegSet(*robot, Leg::B, 0, 800, 1000));
  standUp->push_back(new ESleep(100000));
  standUp->push_back(new ELegSet(*robot, Leg::A, 0, 800, 1000));
  standUp->push_back(new ELegSet(*robot, Leg::B, 0, 800, 1000));
  standUp->push_back(new ESleep(100000));

  standUp->push_back(new ELegSet(*robot, Leg::ALL, 0, -600, 200));

  return standUp;
}


