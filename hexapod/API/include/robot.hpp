#pragma once

#include <list>

#include "serial.hpp"

class Servo {
  public:
    Servo(int, int, bool, int);
    Servo(const Servo &);

    void  updatePosition(int);

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
    Servo shoulder;
    Servo elbow;
    Servo wrist;

  private:
    bool  touch;
    
  public:
    Leg(const Leg &);
    Leg(Servo, Servo, Servo);

    bool getTouch();

    void forward(int);
    void backward(int); 
    void up();
    void down(); 
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

  public: 
    Body(Leg, Leg, Leg, Leg, Leg, Leg);
    Body();

    void commit();
    void turn(int);
    void step(int, int);
};
