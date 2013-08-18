#pragma once

#include "list.hpp"

#include "serial.hpp"
#include "servo.hpp"
#include "leg.hpp"

class Event;

class Body {
  public:
    enum Gait {STOP = 0, TRIPOD = 1};
    enum GaitStatus {STARTGAIT = 0, WALKING = 1, STOPGAIT = 2};
    enum GaitPose {UNKNOWN, AU, AD, AB, BU, BD, BB };

    Gait       gait;
    GaitStatus gaitStatus;
    GaitPose   gaitPose;
    int stepCount;

  public:
    Serial serial;
    int    direction;
    bool   run;
    int    time;

  public:
    int  height;
    int  y;
    int  x;
    int  turn;

    Leg  fr;
    Leg  mr;
    Leg  br;
    Leg  fl;
    Leg  ml;
    Leg  bl;

    list   events;

    static const int     LEGS = 6;
    static const int     SERVOS = 18;

    Servo                *servos[SERVOS];
    Leg                  *legs[6];

  public: 
    Body(Leg&, Leg&, Leg&, Leg&, Leg&, Leg&);
    Body();

    void start();
    void addAction(list *action);
    int commit();

    void setAllLeg(int, int, int);
    void setALeg(int, int, int);
    void setBLeg(int, int, int);
};

extern Body *robot;
