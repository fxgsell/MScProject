#pragma once

class Leg {
  public: 
    enum LegId {ALL   = 0b111111,
		A     = 0b010101,
		B     = 0b101010,
		FRONT = 0b100001,
		MIDLE = 0b010010,
		BACK  = 0b001100,
		RIGHT = 0b111000,
		LEFT  = 0b000111,
		FR    = 0b100000,
		MR    = 0b010000,
		BR    = 0b001000,
		BL    = 0b000100,
		ML    = 0b000010,
		FL    = 0b000001};

  public:
    Leg(const Leg &);
    Leg(LegId, double, Servo, Servo, Servo);

    void save();	//IMPLEMENT IN Leg:: ? (for now call save of servo)
    void restore();
 
    int  setHeight(int);
    void setPosition(int = 0, int = 0, int = 0);
    void setAngles(int = 0, int = 0, int = 0);
    void center();

    void setShoulder(int x);

    int forward(int = 300);
    int backward(int = 300); 
    int up(int = 1);
    int down(int = 1); 

    int updateCoord(double x, double y, double z);
    int setCoord(double x, double y, double z);

    int	  angle;
    int	  hstate;

  private:
    bool touch;
    int	 height;

    int   x;
    int   y;
    int   z;

  public: //TOPRIVATE
    LegId id;
    double angleOffset;
    Servo &shoulder;
    Servo &elbow;
    Servo &wrist;
};


