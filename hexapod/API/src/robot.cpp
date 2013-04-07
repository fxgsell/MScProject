#include <unistd.h>
#include <sstream>

#include "list.hpp"
#include "servo.hpp"
#include "event.hpp"
#include "leg.hpp"
#include "robot.hpp"
#include "socket.hpp"

/*
** Body
*/
Body::Body(Leg fr, Leg mr, Leg br, Leg fl, Leg ml, Leg bl) :
				serial("/dev/ttyAMA0"),
				fr(fr), mr(mr), br(br), fl(fl), ml(ml), bl(bl) {

  if (srv_create(9930) == -1)
    throw ;

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
    int id = servos[i]->getId();

    if (servos[i]->hasChanged()) {
      if (id == 0 || id == 4 || id == 8 || id == 18 || id == 22 || id == 26)
        shoulder << " #" << id << " P" << servos[i]->getRealPosition();
      else if (id == 1 || id == 5 || id == 9 || id == 17 || id == 21 || id == 25)
        elbow << " #" << id << " P" << servos[i]->getRealPosition();
      else if (id == 2 || id == 6 || id == 10 || id == 16 || id == 20 || id == 24)
        wrist << " #" << id << " P" << servos[i]->getRealPosition();
      servos[i]->changeDone();
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
  struct timeval  *tv_ptr;
  int r;
  run = true;

  for (;run;) {    //just for now will be ;;
    init_fd();

    tv_ptr = NULL;
    r = select(lastfd + 1, &fd_read, &fd_write, NULL, tv_ptr);
    check_fd(r);

    if (!events.empty()) { 		//execute next event
      ((Event*)events.start)->execute(); 
      events.pop();
    }
  }
}

void Body::addAction(list *action) {
  events.insert(action);
  delete action;
}

