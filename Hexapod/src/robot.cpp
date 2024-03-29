#include <strings.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sstream>
#include <string.h>
#include <cstdlib>

#include "list.hpp"
#include "servo.hpp"
#include "event.hpp"
#include "leg.hpp"
#include "action.hpp"
#include "robot.hpp"
#include "socket.hpp"

#include <typeinfo>

#define SERIALPORT "/dev/ttyAMA0"
#define TCPPORT 9930

int walk();

extern int fdviewer;

/*
** Body
*/
Body::Body(Leg &fr, Leg &mr, Leg &br, Leg &fl, Leg &ml, Leg &bl) :
				serial(SERIALPORT),
				fr(fr), mr(mr), br(br), fl(fl), ml(ml), bl(bl) {
  if (srv_create(TCPPORT, serial.getfd()) == -1)
    throw ;


  servos[0] = &(this->fr.shoulder);
  servos[1] = &(this->fr.elbow);
  servos[2] = &(this->fr.wrist);

  servos[3] = &(this->mr.shoulder);
  servos[4] = &(this->mr.elbow);
  servos[5] = &(this->mr.wrist);

  servos[6] = &(this->br.shoulder);
  servos[7] = &(this->br.elbow);
  servos[8] = &(this->br.wrist);

  servos[9] = &(this->bl.shoulder);
  servos[10] = &(this->bl.elbow);
  servos[11] = &(this->bl.wrist);

  servos[15] = &(this->fl.shoulder);
  servos[16] = &(this->fl.elbow);
  servos[17] = &(this->fl.wrist);

  servos[12] = &(this->ml.shoulder);
  servos[13] = &(this->ml.elbow);
  servos[14] = &(this->ml.wrist);
  
  legs[0] = &(this->fl);
  legs[1] = &(this->ml);
  legs[2] = &(this->bl);
  legs[3] = &(this->fr);
  legs[4] = &(this->mr);
  legs[5] = &(this->br);

  gait = TRIPOD;
  gaitStatus = STARTGAIT;
  gaitPose = UNKNOWN;

  time = 0;
  x = 0;
  y = 0;
  turn = 0;
  height = 25;

  commit();
}

extern int fdserial;
/*
** Push moves to serial
*/
int Body::commit() {
  int maxTime;
  std::stringstream a;
  std::stringstream viewer;

  a << "S";
  for (int i = 0; i < Body::SERVOS; i++) { 
    int id = servos[i]->getId();

    if (servos[i]->hasChanged()) {
      a << " #" << id << " P" << servos[i]->getRealPosition();
      servos[i]->changeDone();
      if (fdviewer) {
        viewer << " moveLeg " << i/3 << " " << i%3 << " " << servos[i]->getAngle();
      }
    }
  }
  maxTime = 150;
  a << " T" << maxTime << "\x0d";

  char *s = strdup(viewer.str().c_str());
  fds[fdviewer].buf_write.push_back(s);

  s = strdup(a.str().c_str());
  fds[fdserial].buf_write.push_back(s);
  #ifdef DEBUG
  printf("{{%s}}\n", s);
  #endif // DEBUG
  return (maxTime);
}

/*
** Main method: Event Manager
*/

void Body::start() {
  struct timeval  tv;
  struct timeval  tv_cur;
  struct timeval  tv_act;
  struct timeval  tv_nxt;
  int r = 100;
  run = true;
  int i = 0;

  bzero(&tv, sizeof(struct timeval));
  bzero(&tv_act, sizeof(struct timeval));
  bzero(&tv_nxt, sizeof(struct timeval));

  for (;run;) { 
    #ifdef AUTO_WALK // Allow the robot to walk without joystick
    robot->x = 10;
    #endif // AUTO_WALK

    // Calculate the time until the current event is finished
    gettimeofday(&tv_cur, 0);
    timeradd(&tv_cur, &tv_act, &tv_nxt);

    // While it is not time to execute the next action
    // keep reading the socket/serial
    while (timercmp(&tv_cur, &tv_nxt, <) || (!x && !y && !turn && events.empty())) {
      init_fd();
      if (!x && !y && !turn && events.empty())
        r = select(lastfd + 1, &fd_read, &fd_write, NULL, 0);
      else {
        tv.tv_usec = 1000;
        r = select(lastfd + 1, &fd_read, &fd_write, NULL, &tv);
      }
      check_fd(r);
      gettimeofday(&tv_cur, 0);
    }

    if (!events.empty()) {
      Event *e = (Event*)events.pop();
      #ifdef DEBUG
      puts(typeid(*e).name());
      #endif // DEBUG
      tv_act.tv_usec = e->execute();
      delete e;
    }
    else 
      tv_act.tv_usec = walk();

    tv_act.tv_usec *= 1000; //Convert miliseconds to microseconds
    i++;
  }
}

void Body::addAction(list *action) {
  events.insert(action);
  delete action;
}

