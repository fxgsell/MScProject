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
  Servo s0( 0,    75, false, 0);
  Servo s1( 1,  -300, false, 700); // + up
  Servo s2( 2,     0, false, 300); // ERROR
  Servo s4( 4,   -50, false, 0);
  Servo s5( 5,  -150, false, 700); //
  Servo s6( 6,  -300, false, 300); // + in
  Servo s8( 8,   -25, false, 0);
  Servo s9( 9,  -300, false, 700);
  Servo s10(10, -300, false, 300); // + in

  Servo s18(18,   55, true,    0);
  Servo s17(17, -500, true,  700);
  Servo s16(16, -50, true,  300);
  Servo s22(22,  -60, true,    0);
  Servo s21(21, -200, true,  700);
  Servo s20(20,  -50, true,  300);
  Servo s26(26,  -25, true,    0);
  Servo s25(25, -100, true,  700);
  Servo s24(24,  -50, true,  300);

  Leg  fr(Leg::FR, s18, s17, s16);
  Leg  mr(Leg::MR, s22, s21, s20);
  Leg  br(Leg::BR, s26, s25, s24);
  Leg  fl(Leg::FL, s0, s1, s2);
  Leg  ml(Leg::ML, s4, s5, s6);
  Leg  bl(Leg::BL, s8, s9, s10);

  robot = new Body(fr, mr, br, fl, ml, bl);
  robot->commit();



  //robot->events.push_back(new ESleep(1000000));
  //robot->addAction(setLegVLevel(Leg::ALL, 0));
  //robot->events.push_back(new ESleep(1000000));
  //robot->addAction(setLegVLevel(Leg::ALL, 5));
  //robot->events.push_back(new ESleep(1000000));
  //robot->addAction(setLegVLevel(Leg::ALL, 10));
  //robot->events.push_back(new ESleep(1000000));

  robot->addAction(positionWalk());

  robot->start();
  return 0;
}
