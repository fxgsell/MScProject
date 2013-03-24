#pragma once

class Leg {
  public: 
    enum LegGroup {A, B};

  public:
    Leg(const Leg &);
    Leg(LegGroup, Servo, Servo, Servo);

    bool getTouch();
    void setPosition(int = 0, int = 0, int = 0);

    void forward(int = 200);
    void backward(int = 200); 
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


