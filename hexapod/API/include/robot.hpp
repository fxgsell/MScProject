#pragma once

#include <list>

#include "serial.hpp"
#include "servo.hpp"
#include "leg.hpp"

class Event;

class Body {
  private:
    Serial serial;
    int    direction;
    int    speed;

  public:
    Leg  fr;
    Leg  mr;
    Leg  br;
    Leg  fl;
    Leg  ml;
    Leg  bl;

    std::list<Event *>   events;

    Servo                *servos[18];
    Leg                  *legs[6];

    static const int     LEGS = 6;
    static const int     SERVOS = 18;

  public: 
    Body(Leg, Leg, Leg, Leg, Leg, Leg);
    Body();

    void start();
    void addAction(std::list<Event *> *action);
    void commit();

    void setAllLeg(int, int, int);
    void setALeg(int, int, int);
    void setBLeg(int, int, int);
};

extern Body *robot;
