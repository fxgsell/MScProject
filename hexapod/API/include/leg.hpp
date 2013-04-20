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
    Leg(LegId, Servo, Servo, Servo);

    void save();	//IMPLEMENT IN Leg:: ? (for now call save of servo)
    void restore();

    void setPosition(int = 0, int = 0, int = 0);
    void center();

    void forward(int = 300);
    void backward(int = 300); 
    void up(int = 1);
    void down(int = 1); 

    void vLevel(int);
    void setCoord(int x, int y, int z);

  private:
    bool touch;
    int	 height;

    int   x;
    int   y;
    int   z;

  public: //TOPRIVATE
    LegId id;
    Servo shoulder;
    Servo elbow;
    Servo wrist;
};


