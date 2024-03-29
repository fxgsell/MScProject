#pragma once

#include "list.hpp"
#include "event.hpp"

extern Body* robot;

list *standStraight();
list *updateHeight();
list *setLegVLevel(Leg::LegId id, int level);
list *positionWalk();
list *hello(Leg::LegId);
list *standUp();
list *standDown();
list *walk(int steps);
list *flipOver();
list *centerLegs();

