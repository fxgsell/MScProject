#include <cmath>
#include <stdio.h>

#include "servo.hpp"

#define ANGLETOPULSE 10

/*
** Servo
*/

Servo::Servo(const Servo &s): id(s.id), changed(false), adjustment(s.adjustment),
			      invert(s.invert), position(s.position),
			      minAngle(s.minAngle), maxAngle(s.maxAngle) {
}

Servo::Servo(int id, int a, bool i, int min, int max): id(id), changed(false), adjustment(a),
				     invert(i), minAngle(min), maxAngle(max) {
  if (i == true)
    position = -a;
  else
    position = +a;
}

void Servo::updateAngle(double x) {
  angle += x;
  if (angle >= minAngle && angle <= maxAngle)
    setPosition(angle * ANGLETOPULSE);
  else {
    printf("[ERROR] Servo::updateAngle(double x): Invalid servo angle %lf\n", angle + x);
    angle -= x;
  }
}

void Servo::setAngle(double x) {
  if (x >= minAngle && x <= maxAngle) {
    angle = x;
    setPosition(x * ANGLETOPULSE);
  } else {
    printf("[ERROR] Servo::setAngle(double x): Invalid servo angle %lf\n", x);
    printf("maxAngle: %d, minAngle: %d, angle:%d\n", maxAngle, minAngle, (int)x);
  }
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
  int *p = new int;

  *p = position;
  states.push_front(p);
}

void Servo::restore() {
  int *p;

  p = (int *)states.pop();
  if (p) {
    if (*p != position)
      changed = true;
    position = *p;
    delete p;
  }
}

void	Servo::center() {
  position = 1500 + adjustment;
  changed = true;
}
