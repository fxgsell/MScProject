#pragma once

#include <list>

#include "serial.hpp"

class Servo {
  public:
    Servo(int, int, bool, int);
    Servo(const Servo &);

    void  updatePosition(int);
    void  center();

  private:
  public:
    int	  id;
    bool  changed;
    int   adjustment;
    bool  invert;
    int   position; 
};

class Leg {
  public: 
    enum LegGroup {A, B};

  public:
    Leg(const Leg &);
    Leg(LegGroup, Servo, Servo, Servo);

    bool getTouch();

    void forward(int);
    void backward(int); 
    void up();
    void down(); 

  private:
    bool touch;

  public: //TOPRIVATE
    LegGroup group;
    Servo shoulder;
    Servo elbow;
    Servo wrist;
    
};

class Body {
  private:
    Serial serial;

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
};
