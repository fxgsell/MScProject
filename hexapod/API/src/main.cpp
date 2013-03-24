#include <unistd.h>

#include "robot.hpp"

/*
** Initialisation des servos
*/
int main() {
  Servo s0( 0,    75, false, 1500);
  Servo s1( 1,    60, false, 1500); // + up
  Servo s2( 2,     0, false, 1500); // ERROR
  Servo s4( 4,   -45, false, 1500);
  Servo s5( 5,    30, false, 1500); //
  Servo s6( 6,     0, false, 1500); // + in
  Servo s8( 8,   -25, false, 1500);
  Servo s9( 9,     0, false, 1500);
  Servo s10(10,   35, false, 1500); // + in

  Servo s18(18,   55, true, 1500);
  Servo s17(17, -150, true, 1500);
  Servo s16(16,  -50, true, 1500);
  Servo s22(22,  -60, true, 1500);
  Servo s21(21,  -75, true, 1500);
  Servo s20(20,  120, true, 1500);
  Servo s26(26,  -25, true, 1500);
  Servo s25(25,   50, true, 1500);
  Servo s24(24,  -75, true, 1500);

  Leg  fr(Leg::A, s18, s17, s16);
  Leg  mr(Leg::B, s22, s21, s20);
  Leg  br(Leg::A, s26, s25, s24);
  Leg  fl(Leg::B, s0, s1, s2);
  Leg  ml(Leg::A, s4, s5, s6);
  Leg  bl(Leg::B, s8, s9, s10);

  Body *robot = new Body(fr, mr, br, fl, ml, bl);
  robot->commit();

  usleep(1000000);
  robot->step(0, 100);
  usleep(100000);
  return 0;
}
