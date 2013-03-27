#include <unistd.h>
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
  servos[0] = &this->fr.shoulder;
  servos[1] = &this->mr.shoulder;
  servos[2] = &this->br.shoulder;
  servos[3] = &this->fl.shoulder;
  servos[4] = &this->ml.shoulder;
  servos[5] = &this->bl.shoulder;

  servos[6] = &this->fr.elbow;
  servos[7] = &this->mr.elbow;
  servos[8] = &this->br.elbow;
  servos[9] = &this->ml.elbow;
  servos[10] = &this->fl.elbow;
  servos[11] = &this->bl.elbow;

  servos[12] = &this->fr.wrist;
  servos[13] = &this->mr.wrist;
  servos[14] = &this->br.wrist;
  servos[15] = &this->fl.wrist;
  servos[16] = &this->ml.wrist;
  servos[17] = &this->bl.wrist;
  
  legs[0] = (&this->fl);
  legs[1] = (&this->ml);
  legs[2] = (&this->bl);
  legs[3] = (&this->fr);
  legs[4] = (&this->mr);
  legs[5] = (&this->br);
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
  for (int i = 0; i < Body::SERVOS; i++) { 
    int id = servos[i]->id;

    if (servos[i]->changed) {
      if (id == 0 || id == 4 || id == 8 || id == 18 || id == 22 || id == 26)
        shoulder << " #" << servos[i]->id << " P" << servos[i]->getRealPosition();
      else if (id == 1 || id == 5 || id == 9 || id == 17 || id == 21 || id == 25)
        elbow << " #" << servos[i]->id << " P" << servos[i]->getRealPosition();
      else if (id == 2 || id == 6 || id == 10 || id == 16 || id == 20 || id == 24)
        wrist << " #" << servos[i]->id << " P" << servos[i]->getRealPosition();
      servos[i]->changed = false;
    }
  }
  shoulder << "\x0d";
  elbow << "\x0d";
  wrist << "\x0d";

  s = shoulder.str();
  serial.write(s.c_str());
  s = elbow.str();
  serial.write(s.c_str());
  s = wrist.str();
  serial.write(s.c_str());
}

/*
** Event Manager
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

void Body::setAllLeg(int s, int e, int w) {
  for (int i = 0; i < Body::LEGS; i++)
    legs[i]->setPosition(s, e, w);
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
    for (int i = 0; i < Body::LEGS; i++) {
      if (legs[i]->group == Leg::A) {
        legs[i]->up();
        legs[i]->forward();
      }
    }
    commit();
    usleep(100000);
    for (int i = 0; i < Body::LEGS; i++) {
      if (legs[i]->group == Leg::B) {
        legs[i]->backward();
      }
    }
    commit();
    usleep(100000);
    for (int i = 0; i < Body::LEGS; i++) {
      if (legs[i]->group == Leg::A) {
        legs[i]->down();
      }
    }
    commit();
    usleep(100000);
    for (int i = 0; i < Body::LEGS; i++) {
      if (legs[i]->group == Leg::B) {
       legs[i]->up();
       legs[i]->forward();
      }
    }
    commit();
    usleep(100000);
    for (int i = 0; i < Body::LEGS; i++) {
      if (legs[i]->group == Leg::A) {
        legs[i]->backward();
      }
    }
    commit();
    usleep(100000);
    for (int i = 0; i < Body::LEGS; i++) {
      if (legs[i]->group == Leg::B) {
        legs[i]->down();
      }
    }
    commit();
    usleep(100000);

  }
  (void)size;
}

