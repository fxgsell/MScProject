#include <stdio.h>
#include <cmath>

#include "servo.hpp"
#include "leg.hpp"

/*
** Leg Size
*/
const double SIZETIBIA  = 140.0;
const double SIZEFEMUR  = 56.0;
const double SIZECOXA   = 35.0;

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

void Leg::center() {
  shoulder.center();
}

void Leg::setCoord(double x, double y, double z) {
  /*
  double G = sqrt(x * x + z * z);
  double H = sqrt(y * y + G * G);
  double A = SIZEFEMUR;
  double B = SIZETIBIA;

  double b = acos((A * A + H * H - B * B) / (2.0 * A * H));
  double h = asin(H * (sin(b)) / B);
  printf("DEBUG: a=%lf\n", acos((A * A + H * H - B * B) / (2.0 * A * H)));

  double s = atan2(x, z);
  double e = atan(atan2(G, y) + b - 90.0);
  double w = atan(h - 180.0);
  */

  double length = sqrt(x*x + z*z);
  double dist   = sqrt(pow(length - SIZECOXA, 2) + y*y);
  double a1     = atan2((length - SIZECOXA), y);
  double a2     = acos((SIZETIBIA*SIZETIBIA - SIZEFEMUR*SIZEFEMUR - dist*dist) /
                       (-2 * SIZEFEMUR * dist));
  double b1     = acos((dist*dist - SIZETIBIA*SIZETIBIA - SIZEFEMUR*SIZEFEMUR) /
                       (-2 * SIZEFEMUR * SIZETIBIA));

  double s = (atan2(x, z) * 180 / Pi);
  double e = 90 - ((a1 + a2) * 180 / Pi);
  double w = 90 - (b1 * 180 / Pi);

  printf("DEBUG: length=%lf, dist=%lf\n", length, dist);
  printf("DEBUG: a1=%lf, a2=%lf, b1=%lf\n", a1, a2, b1);
  printf("Set: s=%lf, e=%lf, w=%lf\n", s, e, w);

  shoulder.setAngle(s);
  elbow.setAngle(e);
  wrist.setAngle(w);
}

void Leg::setPosition(int s, int e, int w) {
  shoulder.setPosition(s);
  elbow.setPosition(e);
  wrist.setPosition(w);
}

void Leg::forward(int size) {
  shoulder.updatePosition(size);
}

void Leg::backward(int size) {
  shoulder.updatePosition(-size);
}

void Leg::up(int size) {
  // more check
  vLevel(height - size);
  height = height - size;
}

void Leg::down(int size) {
  // more check
  vLevel(height + size);
  height = height + size;
}

void Leg::vLevel(int size) {
  printf ("level %d\n", size);
  if (size == 10) {
    elbow.setPosition(-800);
    wrist.setPosition(-200);
  }
  else if (size == 5) {
    elbow.setPosition(0);
    wrist.setPosition(0);
  }
  else if (size == 2) {
    elbow.setPosition(400);
    wrist.setPosition(300);
  }
  else if (size == 1) {
    elbow.setPosition(800);
    wrist.setPosition(600);
  }
  else if (size == 0) {
    elbow.setPosition(1000);
    wrist.setPosition(600);
  }
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

