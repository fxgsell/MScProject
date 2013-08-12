
#pragma once

#include "ros/ros.h"
#include <ros/message_event.h>

template<typename T>
class Suscriber {
private:
  ros::NodeHandle n;
  ros::Subscriber sub;
public:
  Suscriber(const char* topic, void (*cb)(const ros::MessageEvent<T>&))  {
    sub = n.subscribe("cmd_vel", 1000, cb);
    std::cout << "init suscriber: OK" << std::endl;
}

  ~Suscriber() {}
};
