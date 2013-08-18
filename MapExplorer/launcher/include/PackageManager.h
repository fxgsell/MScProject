
#pragma once

#include "ProcessManager.h"
#include "ros/ros.h"
#include "sensor_msgs/Image.h"
#include "nav_msgs/Odometry.h"
#include "nav_msgs/OccupancyGrid.h"
#include "geometry_msgs/Twist.h"
#include <ros/message_event.h>



class PackageManager {
 private:
  typedef void (*handler)(std::string const &, std::string const&);
  std::map<std::string, handler>          _handler;

 public:

  PackageManager();
  ~PackageManager();
  
  void launchPackage(std::string const &cmd, std::string const &topic, std::string const &type);
  void killThemAll();
};
