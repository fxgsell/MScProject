#pragma once

class Leg {
  public: 
    enum LegGroup {A, B};

  public:
    Leg(const Leg &);
    Leg(LegGroup, Servo, Servo, Servo);

    void save();	//IMPLEMENT IN Leg:: ? (for now call save of servo)
    void restore();

    bool getTouch(); // TODO
    void setPosition(int = 0, int = 0, int = 0);

    void forward(int = 300);
    void backward(int = 300); 
    void up(int = 400);
    void down(int = 400); 

  private:
    bool touch;

  public: //TOPRIVATE
    LegGroup group;
    Servo shoulder;
    Servo elbow;
    Servo wrist;
    
};


