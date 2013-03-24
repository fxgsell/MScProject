#include <unistd.h>

#include "robot.hpp"
#include "leg.hpp"

/*
** Initialisation des servos
*/
int main() {
  Servo s0( 0,    75, false,     0);
  Servo s1( 1,    60, false, 800); // + up
  Servo s2( 2,     0, false,   0); // ERROR
  Servo s4( 4,   -45, false,   0);
  Servo s5( 5,    30, false, 800); //
  Servo s6( 6,     0, false,   0); // + in
  Servo s8( 8,   -25, false,   0);
  Servo s9( 9,     0, false, 800);
  Servo s10(10,   35, false,   0); // + in

  Servo s18(18,   55, true,    0);
  Servo s17(17, -150, true,  800);
  Servo s16(16,  -50, true,    0);
  Servo s22(22,  -60, true,    0);
  Servo s21(21,  -75, true,  800);
  Servo s20(20,  120, true,    0);
  Servo s26(26,  -25, true,    0);
  Servo s25(25,   50, true,  800);
  Servo s24(24,  -75, true,    0);

  Leg  fr(Leg::A, s18, s17, s16);
  Leg  mr(Leg::B, s22, s21, s20);
  Leg  br(Leg::A, s26, s25, s24);
  Leg  fl(Leg::B, s0, s1, s2);
  Leg  ml(Leg::A, s4, s5, s6);
  Leg  bl(Leg::B, s8, s9, s10);

  Body *robot = new Body(fr, mr, br, fl, ml, bl);
/*
  robot->commit();
  usleep(1000000);

  robot->setAllLeg(0, 1000, 1000);
  robot->commit();
  usleep(1000000);
*/

  { // hello();
    robot->fr.setPosition(0, 0, 0);
    robot->commit();
    usleep(500000);
    int i;
    if (true) {
     for (i = 0; i < 4; i++) {
        robot->fr.setPosition(-200, 1000, -600);
        robot->commit();
        usleep(100000);
        robot->fr.setPosition(200, 1000, -400);
        robot->commit();
        usleep(100000);
        robot->fr.setPosition(-200, 1000, -600);
        robot->commit();
        usleep(500000);
      }
      usleep(500000);
      robot->fr.setPosition(0, 0, 0);
      robot->commit();
      usleep(1000000);
    }
  }
  return 0;
}
