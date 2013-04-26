#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "action.hpp"
#include "event.hpp"
#include "robot.hpp"
#include "leg.hpp"
#include "socket.hpp"

Body *robot;

/*
** Initialisation des servos
*/
int main() {
  Servo s0( 0,    50, false);
  Servo s1( 1,  -175, false); // + up
  Servo s2( 2,    50, false); // ERROR
  Servo s4( 4,   -50, false);
  Servo s5( 5,    75, false); //
  Servo s6( 6,    50, false); // + in
  Servo s8( 8,   -25, false);
  Servo s9( 9,     0, false);
  Servo s10(10,  100, false); // + in

  Servo s18(18,   55, true);
  Servo s17(17, -700, true);
  Servo s16(16,  140, true);
  Servo s22(22,  -60, true);
  Servo s21(21,  -25, true);
  Servo s20(20,  150, true);
  Servo s26(26,  -25, true);
  Servo s25(25,  000, true);
  Servo s24(24,   50, true);

  Leg fr(Leg::FR, s18, s17, s16);
  Leg mr(Leg::MR, s22, s21, s20);
  Leg br(Leg::BR, s26, s25, s24);
  Leg fl(Leg::FL, s0, s1, s2);
  Leg ml(Leg::ML, s4, s5, s6);
  Leg bl(Leg::BL, s8, s9, s10);

  robot = new Body(fr, mr, br, fl, ml, bl);

  ELegSetCoord init(*robot, Leg::ALL, 120, 150, 0);
  init.execute();

//  robot->addAction(hello(Leg::A));
//  robot->events.push_back(new ESleep(10000));
//  robot->addAction(hello(Leg::B));
//  robot->addAction(standDown());

  robot->events.push_back(new ELegSetCoord(*robot, Leg::ALL, 120, 150, 0));   // SIDE, VERT, ANGL

  robot->start();

  delete robot;
  return 0;
}
