#include <iostream>
#include <sstream>

#include "robot.hpp"

/*
** Servo
*/

Servo::Servo(const Servo &s): id(s.id), changed(true), adjustment(s.adjustment), invert(s.invert), position(s.position) {
}

Servo::Servo(int id, int a, bool i, int p = 1500): id(id), changed(true), adjustment(a), invert(i) {
  if (i == true)
    position = p - a;
  else
    position = p + a;
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
}

void	Servo::center() {
  position = 1500 + adjustment;
}

/*
** Legs
*/
Leg::Leg(LegGroup g, Servo s, Servo e, Servo w): group(g), shoulder(s), elbow(e), wrist(w) {
}

Leg::Leg(const Leg & l): group(l.group), shoulder(l.shoulder), elbow(l.elbow), wrist(l.wrist) {
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
  elbow.updatePosition(+500);
}

void Leg::down() {
  elbow.updatePosition(-500);
}

/*
** Body
*/
Body::Body(Leg fr, Leg mr, Leg br, Leg fl, Leg ml, Leg bl) :
				serial("/dev/ttyAMA0"),
				fr(fr), mr(mr), br(br), fl(fl), ml(ml), bl(bl) {
  servos.push_back(&this->fr.shoulder);
  servos.push_back(&this->mr.shoulder);
  servos.push_back(&this->br.shoulder);
  servos.push_back(&this->fl.shoulder);
  servos.push_back(&this->ml.shoulder);
  servos.push_back(&this->bl.shoulder);

  servos.push_back(&this->fr.elbow);
  servos.push_back(&this->mr.elbow);
  servos.push_back(&this->br.elbow);
  servos.push_back(&this->ml.elbow);
  servos.push_back(&this->fl.elbow);
  servos.push_back(&this->bl.elbow);

  servos.push_back(&this->fr.wrist);
  servos.push_back(&this->mr.wrist);
  servos.push_back(&this->br.wrist);
  servos.push_back(&this->fl.wrist);
  servos.push_back(&this->ml.wrist);
  servos.push_back(&this->bl.wrist);
  
  legs.push_back(&this->fl);
  legs.push_back(&this->ml);
  legs.push_back(&this->bl);
  legs.push_back(&this->fr);
  legs.push_back(&this->mr);
  legs.push_back(&this->br);
}

void Body::commit() {
  std::string s;
  std::stringstream shoulder;
  std::stringstream elbow;
  std::stringstream wrist;

  shoulder << "S";
  elbow << "S";
  wrist << "S";
  for (std::list<Servo *>::iterator it=servos.begin(); it != servos.end(); ++it) {
    int id = (*it)->id;
    if (id == 0 || id == 4 || id == 8 || id == 18 || id == 22 || id == 26)
      shoulder << " #" << (*it)->id << " P" << (*it)->position;
    else if (id == 1 || id == 5 || id == 9 || id == 17 || id == 21 || id == 25)
      elbow << " #" << (*it)->id << " P" << (*it)->position;
    else if (id == 2 || id == 6 || id == 10 || id == 16 || id == 20 || id == 24)
      wrist << " #" << (*it)->id << " P" << (*it)->position;
  }

  shoulder << "\x0d";
  elbow << "\x0d";
  wrist << "\x0d";

  s = shoulder.str();
  std::cout << s << std::endl;
  serial.write(s);
  s = elbow.str();
  std::cout << s << std::endl;
  serial.write(s);
  s = wrist.str();
  std::cout << s << std::endl;
  serial.write(s);
}

void Body::centerLegs() {
}

void Body::turn(int degree) {
  (void) degree;
}

void Body::step(int size, int number) {

  usleep(100000);
  for (int i = 0; i < number; ++i) {
    for (std::list<Leg *>::iterator it=legs.begin(); it != legs.end(); ++it) {
      if ((*it)->group == Leg::A) {
        (*it)->up();
        (*it)->forward();
      }
    }
    commit();
    usleep(100000);
    for (std::list<Leg *>::iterator it=legs.begin(); it != legs.end(); ++it) {
      if ((*it)->group == Leg::B) {
        (*it)->backward();
      }
    }
    commit();
    usleep(100000);
    for (std::list<Leg *>::iterator it=legs.begin(); it != legs.end(); ++it) {
      if ((*it)->group == Leg::A) {
        (*it)->down();
      }
    }
    commit();
    usleep(100000);
    for (std::list<Leg *>::iterator it=legs.begin(); it != legs.end(); ++it) {
      if ((*it)->group == Leg::B) {
       (*it)->up();
       (*it)->forward();
      }
    }
    commit();
    usleep(100000);
    for (std::list<Leg *>::iterator it=legs.begin(); it != legs.end(); ++it) {
      if ((*it)->group == Leg::A) {
        (*it)->backward();
      }
    }
    commit();
    usleep(100000);
    for (std::list<Leg *>::iterator it=legs.begin(); it != legs.end(); ++it) {
      if ((*it)->group == Leg::B) {
        (*it)->down();
      }
    }
    commit();
    usleep(100000);

  }
  (void)size;


}

