#include <stdio.h>

#include "list.hpp"
#include "action.hpp"
#include "event.hpp"
#include "robot.hpp"


list *hello(Leg::LegId l) {
  list *hello = new list;

  hello->push_back(new ELegSave(*robot, l));
  for (int i = 0; i < 4; i++) {
    hello->push_back(new ELegSet(*robot, l, -200, 1000, -600));
    hello->push_back(new ESleep(100000));
    hello->push_back(new ELegSet(*robot, l, 200, 1000, -400));
    hello->push_back(new ESleep(100000));
    hello->push_back(new ELegSet(*robot, l, -200, 1000, -600));
    hello->push_back(new ESleep(100000));
  }
  hello->push_back(new ESleep(100000));
  hello->push_back(new ELegRestore(*robot, l));
  hello->push_back(new ESleep(100000));

  return hello;
}


list *standUp() {
  list *standUp = new list;

  printf("Goesup\n");
  for (int i = robot->y; i < 27; i++) {   //27
    standUp->push_back(new ELegSetCoord(*robot, Leg::ALL, 120, 40 + i*5, 0));
    robot->y = i;
  }
 
  return standUp;
}

list *standDown() {
  list *standDown = new list;

  for (int i = robot->y; i > 0; i--) { //26
    standDown->push_back(new ELegSetCoord(*robot, Leg::ALL, 120, 40 + i*5, 0));
    robot->y = i;
  }
  return standDown;
}

list *updateHeight() {
  list *updateHeight = new list;

  if (robot->y < robot->height)
    for (int i = robot->y; i < robot->height; i++) { //26
      updateHeight->push_back(new ELegSetCoord(*robot, Leg::ALL, 120, 40 + i*5, 0));
      robot->y = i;
    }
  else
    for (int i = robot->y; i > robot->height - 1; i--) { //26
      updateHeight->push_back(new ELegSetCoord(*robot, Leg::ALL, 120, 40 + i*5, 0));
      robot->y = i;
    }
  return updateHeight;
}

