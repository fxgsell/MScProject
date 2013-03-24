#include "servo.hpp"

/*
** Servo
*/

Servo::Servo(const Servo &s): id(s.id), changed(false), adjustment(s.adjustment), invert(s.invert), position(s.position) {
}

Servo::Servo(int id, int a, bool i, int p = 0): id(id), changed(false), adjustment(a), invert(i) {
  if (i == true)
    position = - p - a;
  else
    position = p + a;
}

int Servo::getRealPosition() {
  return position + 1500;
}

void Servo::setPosition(int p) {
  if (invert == true)
    position = - p - adjustment;
  else
    position = p + adjustment;

  if (position < -1000)
    position = -1000;
  else if (position > 1000)
    position = 1000;

  changed = true;
}

void Servo::updatePosition(int offset) {
  if (invert == true)
    position -= offset;
  else
    position += offset;

  if (position < 100)
    position = 100;
  else if (position > 2500)
    position = 2500;

  if (offset)
    changed = true;
}

void	Servo::center() {
  position = 1500 + adjustment;
  changed = true;
}
