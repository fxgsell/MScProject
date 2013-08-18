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

static const double Pi = 3.14159265358979323846;
/*
** Initialisation des servos
*/
int main() {
  Servo s0( 0,    50, false, -60, 60);
  Servo s1( 1,  -160, false, -30, 90); // + up
  Servo s2( 2,    50, false, -80, 45); // ERROR
  Servo s4( 4,   -50, false, -60, 60);
  Servo s5( 5,    75, false, -30, 90); //
  Servo s6( 6,    30, false, -80, 45); // + in
  Servo s8( 8,   -25, false, -60, 60);
  Servo s9( 9,     0, false, -30, 90);
  Servo s10(10,   50, false, -80, 45); // + in

  Servo s18(18,   55, true, -60, 60);
  Servo s17(17, -100, true, -30, 90);
  Servo s16(16,  120, true, -80, 45);
  Servo s22(22,  -60, true, -60, 60);
  Servo s21(21,  -25, true, -30, 90);
  Servo s20(20,  150, true, -80, 45);
  Servo s26(26,  -25, true, -60, 60);
  Servo s25(25,    0, true, -30, 90);
  Servo s24(24,   50, true, -80, 45);

  Leg fr(Leg::FR, 30, s18, s17, s16);
  Leg mr(Leg::MR,  0, s22, s21, s20);
  Leg br(Leg::BR,-30, s26, s25, s24);
  Leg fl(Leg::FL, 30, s0, s1, s2);
  Leg ml(Leg::ML,  0, s4, s5, s6);
  Leg bl(Leg::BL,-30, s8, s9, s10);

  robot = new Body(fr, mr, br, fl, ml, bl);

  //shoulder -60 to 60 
  //elbow -30 to 90
  //wrist -80 to 30

  //  x = dist from shoulder
  //  y = height    Max:160 Ground:40
  //  z = forward/backward
  //ELegSetAngles init1(*robot, Leg::MIDLE, 0, 90, -80);
  //ELegSetAngles init2(*robot, Leg::FRONT, 0, 90, -80);
  //ELegSetAngles init3(*robot, Leg::BACK , 0, 90, -80);
  ELegSetCoord init1(*robot, Leg::MIDLE, 100, 120, -0);
  ELegSetCoord init2(*robot, Leg::FRONT, 100, 120, -0);
  ELegSetCoord init3(*robot, Leg::BACK , 100, 120, -0);
  init1.execute();
  init2.execute();
  init3.execute();
  
  robot->start();

  delete robot;
  return 0;
}

