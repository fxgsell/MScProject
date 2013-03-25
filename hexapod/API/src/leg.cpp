#include "servo.hpp"
#include "leg.hpp"

/*
** Legs
*/
Leg::Leg(LegGroup g, Servo s, Servo e, Servo w): group(g), shoulder(s), elbow(e), wrist(w) {
}

Leg::Leg(const Leg & l): group(l.group), shoulder(l.shoulder), elbow(l.elbow), wrist(l.wrist) {
}

void Leg::setPosition(int s, int e, int w) {
  shoulder.setPosition(s);
  elbow.setPosition(e);
  wrist.setPosition(w);
}

bool Leg::getTouch() {
  return touch;
}

void Leg::forward(int size) {
  shoulder.updatePosition(size);
}

void Leg::backward(int size) {
  shoulder.updatePosition(-size);
}

void Leg::up(int size) {
  elbow.updatePosition(size);
}

void Leg::down(int size) {
  elbow.updatePosition(-size);
}


void Leg::save() {
  elbow.save();
  shoulder.save();
  wrist.save();
}

void Leg::restore() {
  elbow.restore();
  shoulder.restore();
  wrist.restore();
}

