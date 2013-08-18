#pragma once

#include "robot.hpp"

extern Body* robot;

class Event {
  public:
    virtual int execute() = 0;
    virtual ~Event() {};
};

//should be waiting in poll
class ESleep : public Event {
  public:
    ESleep(int t) : time(t) {};
    int execute();

  private:
    int time;
};

/*
** Do next step
*/
class EStep : public Event {
  public:
    EStep(Body &b) : robot(b) {};
    int execute();

  private:
    Body &robot;
};


/*
** Set Coord
*/
class ELegSetCoord : public Event {
  public:
    ELegSetCoord(Body &b, Leg::LegId g, int x, int y, int z) : robot(b), group(g),  x(x), y(y), z(z) {};
    int execute();

  private:
    Body &robot;
    Leg::LegId	group;
    int  x;
    int  y;
    int  z;
};


/*
** Set
*/
class ELegSetAngles : public Event {
  public:
    ELegSetAngles(Body &b, Leg::LegId g, int s, int e, int w) : robot(b), group(g),  s(s), e(e), w(w) {};
    int execute();

  private:
    Body &robot;
    Leg::LegId	group;
    int  s;
    int  e;
    int  w;
};


/*
** Set
*/
class ELegSet : public Event {
  public:
    ELegSet(Body &b, Leg::LegId g, int s, int e, int w) : robot(b), group(g),  s(s), e(e), w(w) {};
    int execute();

  private:
    Body &robot;
    Leg::LegId	group;
    int  s;
    int  e;
    int  w;
};

/*
** Center
*/

class ELegCenter : public Event {
  public:
    ELegCenter(Body &b, Leg::LegId g) : robot(b), group(g) {};
    int execute();

  private:
    Body &robot;
    Leg::LegId	group;
};


/*
** Down A, B
*/

class ELegDown : public Event {
  public:
    ELegDown(Body &b, Leg::LegId g, int r) : robot(b), group(g), range(r) {};
    int execute();

  private:
    Body &robot;
    Leg::LegId	group;
    int  range;
};

class ELegVLevel : public Event {
  public:
    ELegVLevel(Body &b, Leg::LegId g, int r) : robot(b), group(g), range(r) {};
    int execute();

  private:
    Body &robot;
    Leg::LegId	group;
    int  range;
};

/*
** Up A, B
*/

class ELegUp : public Event {
  public:
    ELegUp(Body &b, Leg::LegId g, int r) : robot(b), group(g), range(r) {};
    int execute();

  private:
    Body &robot;
    Leg::LegId	group;
    int  range;
};

/*
** Forward A, B
*/

class ELegForward : public Event {
  public:
    ELegForward(Body &b, Leg::LegId g, int r) : robot(b), group(g), range(r) {};
    int execute();

  private:
    Body &robot;
    Leg::LegId	group;
    int  range;
};

/*
** Backward A, B
*/

class ELegBackward : public Event {
  public:
    ELegBackward(Body &b, Leg::LegId g, int r) : robot(b), group(g), range(r) {};
    int execute();

  private:
    Body &robot;
    Leg::LegId	group;
    int  range;
};

/*
** Backup
*/

class ELegSave : public Event {
  public:
    ELegSave(Body &b, Leg::LegId g) : robot(b), group(g) {};
    int execute();

  private:
    Body &robot;
    Leg::LegId	group;
};

class ELegRestore : public Event {
  public:
    ELegRestore(Body &b, Leg::LegId g) : robot(b), group(g) {};
    int execute();

  private:
    Body &robot;
    Leg::LegId	group;
};

