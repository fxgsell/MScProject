#pragma once

#include "list.hpp"

class Servo {
  public:
    Servo(int, int, bool);
    Servo(const Servo &);

    int   getRealPosition();
    int   getId();
    bool  hasChanged();
    void  changeDone();
    void  updatePosition(int);
    void  setAngle(double);
    void  updateAngle(double);
    void  setPosition(int);
    void  center();

    void  save();
    void  restore();

  private:
    list            states;
    int	            id;
    bool            changed;
    int             adjustment;
    bool            invert;
    int             position; 
    int             angle;
};


