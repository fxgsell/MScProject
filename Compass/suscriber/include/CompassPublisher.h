
#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/Imu.h>
#include <boost/assign/list_of.hpp>
#include <math.h>

#pragma once

class CompassPublisher {
 private:
  ros::NodeHandle *n;
  ros::Publisher imu_pub;
  tf::TransformBroadcaster imu_broadcaster;
  ros::Time current_time;
  ros::Time last_time;
public:
  CompassPublisher(int, char **, std::string const &name = "compass_publisher");
  ~CompassPublisher();
  
  void publish(double w, double x, double y, double z);
};