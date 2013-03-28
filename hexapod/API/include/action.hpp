#pragma once

#include <list>

#include "event.hpp"

std::list<Event *> *standUp();
std::list<Event *> *standDown();
std::list<Event *> *walk(int steps);
std::list<Event *> *flipOver();
std::list<Event *> *centerLegs();

