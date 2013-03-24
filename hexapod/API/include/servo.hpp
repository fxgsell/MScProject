#pragma once

class Servo {
  public:
    Servo(int, int, bool, int);
    Servo(const Servo &);

    int  getRealPosition();
    void  updatePosition(int);
    void  setPosition(int);
    void  center();

  private:
  public:
    int	  id;
    bool  changed;
    int   adjustment;
    bool  invert;
    int   position; 
};


