
#pragma once

#include "ros/ros.h"
#include <ros/message_event.h>

template<typename T>
class Suscriber {
private:
  ros::NodeHandle n;
  ros::Subscriber sub;
public:
  Suscriber(std::string const &topic, void (*cb)(const ros::MessageEvent<T>&))  {
    sub = n.subscribe(topic.c_str(), 1000, cb);
    if (sub)
      std::cout << "init suscriber: OK" << std::endl;
    else
      std::cout << "fail suscriber..." << std::endl;
}

  ~Suscriber() {}
};
