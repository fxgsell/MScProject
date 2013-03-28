#include "servo.hpp"

/*
** Servo
*/

Servo::Servo(const Servo &s): id(s.id), changed(true), adjustment(s.adjustment), invert(s.invert), position(s.position) {
}

Servo::Servo(int id, int a, bool i, int p = 0): id(id), changed(true), adjustment(a), invert(i) {
  if (i == true)
    position = - p - a;
  else
    position = p + a;
}

void Servo::changeDone() {
  changed = false;
}


bool Servo::hasChanged() {
  return changed;
}

int Servo::getId() {
  return id;
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

  if (position < -1000)
    position = -1000;
  else if (position > 1000)
    position = 1000;

  if (offset)
    changed = true;
}

void Servo::save() {
  states.push(position);
}

void Servo::restore() {
  if (position != states.top())
    changed = true;
  position = states.top();
  states.pop();
}

void	Servo::center() {
  position = 1500 + adjustment;
  changed = true;
}
