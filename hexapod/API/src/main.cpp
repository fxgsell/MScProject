#include <unistd.h>

#include "action.hpp"
#include "event.hpp"
#include "robot.hpp"
#include "leg.hpp"

Body *robot;

/*
** Initialisation des servos
*/
int main() {
  Servo s0( 0,    75, false, 0);
  Servo s1( 1,    60, false, 700); // + up
  Servo s2( 2,     0, false, 300); // ERROR
  Servo s4( 4,   -45, false, 0);
  Servo s5( 5,    30, false, 700); //
  Servo s6( 6,     0, false, 300); // + in
  Servo s8( 8,   -25, false, 0);
  Servo s9( 9,     0, false, 700);
  Servo s10(10,   35, false, 300); // + in

  Servo s18(18,   55, true,    0);
  Servo s17(17, -150, true,  700);
  Servo s16(16,  -50, true,  300);
  Servo s22(22,  -60, true,    0);
  Servo s21(21,  -75, true,  700);
  Servo s20(20,  120, true,  300);
  Servo s26(26,  -25, true,    0);
  Servo s25(25,   50, true,  700);
  Servo s24(24,  -75, true,  300);

  Leg  fr(Leg::FR, s18, s17, s16);
  Leg  mr(Leg::MR, s22, s21, s20);
  Leg  br(Leg::BR, s26, s25, s24);
  Leg  fl(Leg::FL, s0, s1, s2);
  Leg  ml(Leg::ML, s4, s5, s6);
  Leg  bl(Leg::BL, s8, s9, s10);

  robot = new Body(fr, mr, br, fl, ml, bl);

/*
  robot->events.push_back(new ESleep(1000000));
  robot->addAction(standUp());
  robot->addAction(standDown());
  robot->addAction(walk(2));
*/
  robot->addAction(flipOver());
  robot->start();

/*
  robot->standUp();
  usleep(1000000);
  robot->hello();
  usleep(1000000);
  robot->standDown();
  usleep(1000000);

  robot->mr.up();
  robot->ml.up();
  robot->commit();
  usleep(1000000);

  robot->step(0, 6);
  usleep(1000000);

  robot->standDown();
  usleep(1000000);

  robot->setAllLeg(0, 700,300);
  usleep(1000000);
*/

  return 0;
}
