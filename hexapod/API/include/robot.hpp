#pragma once

#include "list.hpp"

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

    list   events;

    Servo                *servos[18];
    Leg                  *legs[6];

    static const int     LEGS = 6;
    static const int     SERVOS = 18;

  public: 
    Body(Leg, Leg, Leg, Leg, Leg, Leg);
    Body();

    void start();
    void addAction(list *action);
    void commit();

    void setAllLeg(int, int, int);
    void setALeg(int, int, int);
    void setBLeg(int, int, int);
};

extern Body *robot;
