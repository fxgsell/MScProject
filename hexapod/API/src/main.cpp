#include "robot.hpp"

/*
** Initialisation des servos
*/
int main() {
  Servo s0(0, 0, false, 1500);
  Servo s1(1, 0, false, 1500);
  Servo s2(2, 0, false, 1500);
  Servo s4(3, 0, false, 1500);
  Servo s5(5, 0, false, 1500);
  Servo s6(6, 0, false, 1500);
  Servo s8(8, 0, false, 1500);
  Servo s9(9, 0, false, 1500);
  Servo s10(10, 0, false, 1500);

  Servo s18(18, 0, false, 1500);
  Servo s17(17, 0, false, 1500);
  Servo s16(16, 0, false, 1500);
  Servo s22(22, 0, false, 1500);
  Servo s21(21, 0, false, 1500);
  Servo s20(20, 0, false, 1500);
  Servo s26(26, 0, false, 1500);
  Servo s25(25, 0, false, 1500);
  Servo s24(24, 0, false, 1500);

  Leg  fr(s18, s17, s16);
  Leg  mr(s22, s21, s20);
  Leg  br(s26, s25, s24);
  Leg  fl(s0, s1, s2);
  Leg  ml(s4, s5, s6);
  Leg  bl(s8, s8, s10);

  Body *robot = new Body(fr, mr, br, fl, ml, bl);
  robot->commit();

  return 0;
}
