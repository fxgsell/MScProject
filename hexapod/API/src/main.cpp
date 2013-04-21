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
  Servo s0( 0,    50, false, 0);
  Servo s1( 1,  -125, false, 1000); // + up
  Servo s2( 2,    50, false, 600); // ERROR
  Servo s4( 4,   -50, false, 0);
  Servo s5( 5,    75, false,1000); //
  Servo s6( 6,    50, false, 600); // + in
  Servo s8( 8,   -25, false, 0);
  Servo s9( 9,     0, false,1000);
  Servo s10(10,  100, false, 600); // + in

  Servo s18(18,   55, true,    0);
  Servo s17(17, -600, true, 1000);
  Servo s16(16,  140, true,  600);
  Servo s22(22,  -60, true,    0);
  Servo s21(21,  -25, true, 1000);
  Servo s20(20,  150, true,  600);
  Servo s26(26,  -25, true,    0);
  Servo s25(25,  25, true, 1000);
  Servo s24(24,   50, true,  600);

  Leg  fr(Leg::FR, s18, s17, s16);
  Leg  mr(Leg::MR, s22, s21, s20);
  Leg  br(Leg::BR, s26, s25, s24);
  Leg  fl(Leg::FL, s0, s1, s2);
  Leg  ml(Leg::ML, s4, s5, s6);
  Leg  bl(Leg::BL, s8, s9, s10);

  robot = new Body(fr, mr, br, fl, ml, bl);

//  robot->events.push_back(new ELegSetAngles(*robot, Leg::ALL, 0, 0, -90));

  robot->start();

  return 0;
}
