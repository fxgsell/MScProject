#include "ros/ros.h"
#include "geometry_msgs/Quaternion.h"

#include <sstream>

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{
  ros::init(argc, argv, "talker");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<geometry_msgs::Quaternion>("chatter", 1000);

  ros::Rate loop_rate(10);

  int count = 0;
  while (ros::ok()) {
    geometry_msgs::Quaternion data;
    
    data.x = 42;
    data.y = 4242;
    data.z = 26;
    data.w = 51;
    chatter_pub.publish(data);
    
    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }

  return 0;
}
