#include <unistd.h>
#include <iostream>
#include <sstream>

#include "servo.hpp"
#include "event.hpp"
#include "leg.hpp"
#include "robot.hpp"

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

/*
** Push moves to serial
*/
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

    if ((*it)->changed) {
      if (id == 0 || id == 4 || id == 8 || id == 18 || id == 22 || id == 26)
        shoulder << " #" << (*it)->id << " P" << (*it)->getRealPosition();
      else if (id == 1 || id == 5 || id == 9 || id == 17 || id == 21 || id == 25)
        elbow << " #" << (*it)->id << " P" << (*it)->getRealPosition();
      else if (id == 2 || id == 6 || id == 10 || id == 16 || id == 20 || id == 24)
        wrist << " #" << (*it)->id << " P" << (*it)->getRealPosition();
      (*it)->changed = false;
    }
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

/*
** Event Manager

void Body::executeNextEvent() {
  events.pop_front()->execute();
}
*/

void Body::start() {
  for (;!events.empty();) {    //just for now will be ;;
    // read input
    // read sensors
    if (!events.empty()) { 		//execute next event
      events.front()->execute(); 
      delete events.front();
      events.pop_front();
    }
    // sleep on pool
  }
}

void Body::addAction(std::list<Event *> *action) {
  events.insert(events.end(), action->begin(), action->end());
  delete action;
}

/*
** Varrious --not so useful-- moves
*/
void Body::centerLegs() {
}

void Body::sit() {
}

void Body::setALeg(int s, int e, int w) {
  for (std::list<Leg *>::iterator it=legs.begin(); it != legs.end(); ++it) {
    if ((*it)->group == Leg::A)
    (*it)->setPosition(s, e, w);
  }
  commit();
}

void Body::setBLeg(int s, int e, int w) {
  for (std::list<Leg *>::iterator it=legs.begin(); it != legs.end(); ++it) {
    if ((*it)->group == Leg::B)
      (*it)->setPosition(s, e, w);
  }
  commit();
}

void Body::setAllLeg(int s, int e, int w) {
  for (std::list<Leg *>::iterator it=legs.begin(); it != legs.end(); ++it) {
    (*it)->setPosition(s, e, w);
  }
  commit();
}

void Body::standDown() {
  setAllLeg(0, 300, 300);
  commit();
}

void Body::hello() {
   bl.save();
   bl.setPosition(0, 0, 0);
   commit();
   int i;
   for (i = 0; i < 4; i++) {
     bl.setPosition(-200, 1000, -600);
     commit();
     usleep(100000);
     bl.setPosition(200, 1000, -400);
     commit();
     usleep(100000);
     bl.setPosition(-200, 1000, -600);
     commit();
     usleep(100000);
   }
   usleep(300000);
   bl.restore();
   commit();
   usleep(100000);
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

