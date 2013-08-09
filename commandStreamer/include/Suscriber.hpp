
#pragma once

#include "ros/ros.h"
#include <ros/message_event.h>

template<typename T>
class Suscriber {
 public:
  Suscriber(const char *base_name, int argc, char **argv) {
  ros::init(argc, argv, base_name);
}

  void init(const char* topic, 
	    void (*cb)(const ros::MessageEvent<T>&))  {
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe(topic, 1000, cb);
  ros::spin();
}

  ~Suscriber() {}
};
