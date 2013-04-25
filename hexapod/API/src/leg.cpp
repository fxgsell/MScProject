#include <stdio.h>
#include <cmath>

#include "servo.hpp"
#include "leg.hpp"

/*
** Leg Size
*/
const double SIZETIBIA  = 141.0;
const double SIZEFEMUR  = 58.0;
const double SIZECOXA   = 37.0;

const double Pi = 3.14159265358979323846;

/*
** Legs
*/
Leg::Leg(LegId i, Servo s, Servo e, Servo w): id(i), shoulder(s), elbow(e), wrist(w) {
 height = 0;
}

Leg::Leg(const Leg & l): id(l.id), shoulder(l.shoulder), elbow(l.elbow), wrist(l.wrist) {
 height = 0;
}

void Leg::setShoulder(int x) {
  if (id & RIGHT)
    angle = x;
  else
    angle = -x;
}

void Leg::setAngles(int s, int e, int w) {
  shoulder.setAngle(s);
  elbow.setAngle(e);
  wrist.setAngle(w);
}

int Leg::updateCoord(double x, double y, double z) {
  return (setCoord(this->x + x, this->y + y, this->z + z));
}

int Leg::setCoord(double x, double y, double z) {
  double length = sqrt(x*x + z*z);
  double dist   = sqrt(pow(length - SIZECOXA, 2) + y*y);
  double a1     = atan2((length - SIZECOXA), y);
  double a2     = acos((SIZETIBIA*SIZETIBIA - SIZEFEMUR*SIZEFEMUR - dist*dist) /
                       (-2 * SIZEFEMUR * dist));
  double b1     = acos((dist*dist - SIZETIBIA*SIZETIBIA - SIZEFEMUR*SIZEFEMUR) /
                       (-2 * SIZEFEMUR * SIZETIBIA));

  double s = 90 - (atan2(x, z) * 180 / Pi);
  double e = -90 + ((a1 + a2) * 180 / Pi);
  double w = 90 - (b1 * 180 / Pi);
  

  if (s == s && e == e && w == w) {
    shoulder.setAngle(s + angle);
    elbow.setAngle(e);
    wrist.setAngle(w);
    this->x = x;
    this->y = y;
    this->z = z;
  }  else {
    printf("DEBUG: length=%lf, dist=%lf\n", length, dist);
    printf("DEBUG: a1=%lf, a2=%lf, b1=%lf\n", a1, a2, b1);
    printf("Set: s=%lf, e=%lf, w=%lf\n", s, e, w);
    return 1;
  }
  return 0;
}

void Leg::setPosition(int s, int e, int w) {
  shoulder.setPosition(s);
  elbow.setPosition(e);
  wrist.setPosition(w);
}

int Leg::forward(int size) {
  return setCoord(x, y, z - size);
}

int Leg::backward(int size) {
  return setCoord(x, y, z + size);
}

int Leg::up(int size) {
  return setCoord(x, y - size, z);
}

int Leg::down(int size) {
  return setCoord(x, y + size, z);
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

