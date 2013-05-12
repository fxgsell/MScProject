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

static const double Pi = 3.14159265358979323846;

/*
** Legs
*/
Leg::Leg(LegId i, double a, Servo s, Servo e, Servo w):
	id(i), angleOffset(a), shoulder(s), elbow(e), wrist(w) {
 height = 0;
 x = 120;
 y = 150;
 z = 0;
 hstate = 0;
}

Leg::Leg(const Leg & l):
	id(l.id), angleOffset(l.angleOffset), shoulder(l.shoulder),
	elbow(l.elbow), wrist(l.wrist) {
 height = 0;
 x = 120;
 y = 150;
 z = 0;
 hstate = 0;
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

int Leg::setCoord(double px, double py, double pz) {

  double x = px;
  double z = pz;
  double y = py;

  double length = sqrt(x*x + z*z);
  double dist   = sqrt(pow(length - SIZECOXA, 2.0) + y*y);
  double a1     = atan2((length - SIZECOXA), y);
  double a2     = acos((SIZEFEMUR*SIZEFEMUR + dist*dist - SIZETIBIA*SIZETIBIA) /
                       (2.0 * SIZEFEMUR * dist));
  double b1     = acos((SIZETIBIA*SIZETIBIA + SIZEFEMUR*SIZEFEMUR - dist*dist) /
                       (2.0 * SIZEFEMUR * SIZETIBIA));

  double rs = atan2(x, z);
  double re = a1 + a2;
  double rw = b1;

  double s =  90 - (rs * 180 / Pi);
  double e = -90 + (re * 180 / Pi);
  double w =  90 - (rw * 180 / Pi);
  

  if (s == s && e == e && w == w) {
    shoulder.setAngle(s + angle + angleOffset);
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
  updateCoord(0, 0, -(hstate/2 + size/2));
  hstate = size;
}

int Leg::backward(int size) {
  updateCoord(0, 0, (hstate/2 + size/2));
  hstate = size;
}

int Leg::setHeight(int size) {
  updateCoord(x, size, z);
}

int Leg::up(int size) {
  updateCoord(0, -size, 0);
}

int Leg::down(int size) {
  updateCoord(0, size, 0);
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

