#include <stdio.h>
#include <cmath>

#include "servo.hpp"
#include "leg.hpp"

/*
** Leg Size
*/
const double SIZETIBIA  = 140.0;
const double SIZEFEMUR  = 56.0;

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
  double dist = sqrt(x * x + z * z);
  double b = sqrt(z*z+dist*dist);
  double T1 = atan2(dist, z);
  double Ti = acos((SIZEFEMUR*SIZEFEMUR + SIZETIBIA*SIZETIBIA - b*b)/(2*SIZEFEMUR*SIZETIBIA));

  printf("DEBUG: %lF / %lF = %lF\n", (SIZEFEMUR*SIZEFEMUR + SIZETIBIA*SIZETIBIA - b*b), (2*SIZEFEMUR*SIZETIBIA),  (SIZEFEMUR*SIZEFEMUR + SIZETIBIA*SIZETIBIA - b*b) / (2*SIZEFEMUR*SIZETIBIA)); 
  printf("DEBUG: acos(%lF) = %lF\n", 1.43, acos(1.43));
  double s = atan2(x, z);
  double e = T1;
  double w = Ti;
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

  printf("Set: b=%lf, h=%lf\n", b, h);
  printf("Set: G=%lf, H=%lf, A=%lf, B=%lf\n", G, H, A, B);
  printf("Set: s=%lf, e=%lf, w=%lf\n", s, e, w);
*/
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

