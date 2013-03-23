#include <iostream>
#include <sstream>

#include "robot.hpp"

/*
** Servo
*/

Servo::Servo(const Servo & s): id(s.id), adjustment(s.adjustment), invert(s.invert), position(s.position), changed(true) {
}

Servo::Servo(int id, int a, bool i, int p = 1500): id(id), adjustment(a), invert(i), position(p), changed(true) {
}

void Servo::updatePosition(int offset) {
  position += offset;
  if (position < 100)
    position = 100;
  else if (position > 2500)
    position = 2500;
}

/*
** Legs
*/
Leg::Leg(Servo s, Servo e, Servo w): shoulder(s), elbow(e), wrist(w) {
}

Leg::Leg(const Leg & l): shoulder(l.shoulder), elbow(l.elbow), wrist(l.wrist) {
}

bool Leg::getTouch() {
  return touch;
}

void Leg::forward(int size = 200) {
  shoulder.updatePosition(size);
}

void Leg::backward(int size = 200) {
  shoulder.updatePosition(-size);
}

void Leg::up() {
  elbow.updatePosition(+200);
}

void Leg::down() {
  elbow.updatePosition(-200);
}

/*
** Body
*/
Body::Body(Leg fr, Leg mr, Leg br, Leg fl, Leg ml, Leg bl) :
				fr(fr), mr(mr), br(br), fl(fl), ml(ml), bl(bl),
				serial("/dev/ttyAMA0") {
  servos.push_back(&fr.shoulder);
  servos.push_back(&fr.elbow);
  servos.push_back(&fr.wrist);
  servos.push_back(&mr.shoulder);
  servos.push_back(&mr.elbow);
  servos.push_back(&mr.wrist);
  servos.push_back(&br.shoulder);
  servos.push_back(&br.elbow);
  servos.push_back(&br.wrist);

  servos.push_back(&fl.shoulder);
  servos.push_back(&fl.elbow);
  servos.push_back(&fl.wrist);
  servos.push_back(&ml.shoulder);
  servos.push_back(&ml.elbow);
  servos.push_back(&ml.wrist);
  servos.push_back(&bl.shoulder);
  servos.push_back(&bl.elbow);
  servos.push_back(&bl.wrist);
  
}

void Body::commit() {
  std::stringstream buf;

  buf << "S";
  for (std::list<Servo *>::iterator it=servos.begin(); it != servos.end(); ++it)
    buf << " #" << (*it)->id << " P" << (*it)->position;
  buf << "\x0d" << std::endl;

  std::string s = buf.str();
  serial.write(s);
}

void Body::turn(int degree) {
}

void Body::step(int size, int number) {
  for (; number > 0; --number) {
    ;
  }
}
