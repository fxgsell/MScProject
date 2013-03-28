#pragma once

#include <stack>

class Servo {
  public:
    Servo(int, int, bool, int);
    Servo(const Servo &);

    int   getRealPosition();
    int   getId();
    bool  hasChanged();
    void  changeDone();
    void  updatePosition(int);
    void  setPosition(int);
    void  center();

    void  save();
    void  restore();

  private:
    std::stack<int> states;
    int	            id;
    bool            changed;
    int             adjustment;
    bool            invert;
    int             position; 
};


