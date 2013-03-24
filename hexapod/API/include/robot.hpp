#pragma once

#include <list>

#include "serial.hpp"
#include "servo.hpp"
#include "leg.hpp"

class Body {
  private:
    Serial serial;

  public:
    Leg  fr;
    Leg  mr;
    Leg  br;
    Leg  fl;
    Leg  ml;
    Leg  bl;

    std::list<Servo *> servos;
    std::list<Leg *> legs;

  public: 
    Body(Leg, Leg, Leg, Leg, Leg, Leg);
    Body();

    void centerLegs();
    void commit();
    void turn(int);
    void step(int, int);

    void setAllLeg(int, int, int);
    void sit();
    void standUp();
    void standDown();

    void heightUp(int);
    void heightDown(int);
};
