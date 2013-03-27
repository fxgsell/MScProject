#pragma once

class Leg {
  public: 
    enum LegId {ALL = 0b111111,
		A   = 0b010101,
		B   = 0b101010,
		FR  = 0b100000,
		MR  = 0b010000,
		BR  = 0b001000,
		BL  = 0b000100,
		ML  = 0b000010,
  		FL  = 0b000001};

  public:
    Leg(const Leg &);
    Leg(LegId, Servo, Servo, Servo);

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
    LegId id;
    Servo shoulder;
    Servo elbow;
    Servo wrist;
};


