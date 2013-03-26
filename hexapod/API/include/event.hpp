#pragma once

#include "robot.hpp"
//typedef std::list<Event *>;

class Event {
  public:
    virtual int execute() = 0;
    virtual ~Event() {};

  private:
};

class ESleep : public Event {
  public:
    ESleep(int t) : time(t) {};
    int execute();

  private:
    int time;
};

class EStandDown : public Event {
  public:
    EStandDown(Body &r) : robot(r) {};
    int execute();

  private:
    Body &robot;
};


class ESetAllLeg : public Event {
  public:
    ESetAllLeg(Body &r, int s, int e, int w) : robot(r), s(s), e(e), w(w) {};
    int execute();

  private:
    Body &robot;
    int  s;
    int  e;
    int  w;
};


class ESetALeg : public Event {
  public:
    ESetALeg(Body &r, int s, int e, int w) : robot(r), s(s), e(e), w(w) {};
    int execute();

  private:
    Body &robot;
    int  s;
    int  e;
    int  w;
};


class ESetBLeg : public Event {
  public:
    ESetBLeg(Body &r, int s, int e, int w) : robot(r), s(s), e(e), w(w) {};
    int execute();

  private:
    Body &robot;
    int  s;
    int  e;
    int  w;
};


