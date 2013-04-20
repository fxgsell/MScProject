#include <stdio.h>
#include <cmath>

#include "servo.hpp"
#include "leg.hpp"

/*
** Leg Size
*/
#define SIZETIBIA 60
#define SIZEFEMUR 100

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

void Leg::setCoord(int x, int y, int z) {
  int G = sqrt(x * x + z * z);
  int H = sqrt(y * y + G * G);
  int A = SIZEFEMUR;
  int B = SIZETIBIA;

  int b = acos((A * A + H * H - B * B) / (2 * A * H));
  int h = asin(H * (sin(b)) / B);

  int s = atan(x / z);
  int e = atan(atan(G / y) + b - 90);
  int w = atan(h - 180);

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

