#pragma once

#include "robot.hpp"
//typedef std::list<Event *>;

class Event {
  public:
    virtual int execute() = 0;
    virtual ~Event() {};

  private:
};

//should be waiting in poll
class ESleep : public Event {
  public:
    ESleep(int t) : time(t) {};
    int execute();

  private:
    int time;
};

//Should not be an event
class EStandDown : public Event {
  public:
    EStandDown(Body &b) : robot(b) {};
    int execute();

  private:
    Body &robot;
};


class ESetAllLeg : public Event {
  public:
    ESetAllLeg(Body &b, int s, int e, int w) : robot(b), s(s), e(e), w(w) {};
    int execute();

  private:
    Body &robot;
    int  s;
    int  e;
    int  w;
};


class ESetALeg : public Event {
  public:
    ESetALeg(Body &b, int s, int e, int w) : robot(b), s(s), e(e), w(w) {};
    int execute();

  private:
    Body &robot;
    int  s;
    int  e;
    int  w;
};


class ESetBLeg : public Event {
  public:
    ESetBLeg(Body &b, int s, int e, int w) : robot(b), s(s), e(e), w(w) {};
    int execute();

  private:
    Body &robot;
    int  s;
    int  e;
    int  w;
};

/*
** Down A, B
*/

class EDownALeg : public Event {
  public:
    EDownALeg(Body &b, int r) : robot(b), range(r) {};
    int execute();

  private:
    Body &robot;
    int  range;
};

class EDownBLeg : public Event {
  public:
    EDownBLeg(Body &b, int r) : robot(b), range(r) {};
    int execute();

  private:
    Body &robot;
    int  range;
};


/*
** Up A, B
*/

class EUpALeg : public Event {
  public:
    EUpALeg(Body &b, int r) : robot(b), range(r) {};
    int execute();

  private:
    Body &robot;
    int  range;
};

class EUpBLeg : public Event {
  public:
    EUpBLeg(Body &b, int r) : robot(b), range(r) {};
    int execute();

  private:
    Body &robot;
    int  range;
};

/*
** Forward A, B
*/

class EForwardALeg : public Event {
  public:
    EForwardALeg(Body &b, int r) : robot(b), range(r) {};
    int execute();

  private:
    Body &robot;
    int  range;
};

class EForwardBLeg : public Event {
  public:
    EForwardBLeg(Body &b, int r) : robot(b), range(r) {};
    int execute();

  private:
    Body &robot;
    int  range;
};

/*
** Backward A, B
*/

class EBackwardALeg : public Event {
  public:
    EBackwardALeg(Body &b, int r) : robot(b), range(r) {};
    int execute();

  private:
    Body &robot;
    int  range;
};

class EBackwardBLeg : public Event {
  public:
    EBackwardBLeg(Body &b, int r) : robot(b), range(r) {};
    int execute();

  private:
    Body &robot;
    int  range;
};


