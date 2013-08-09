
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <ros/message_event.h>
#include "protocol.h"
#include "Suscriber.hpp"
#include "commandStreamerSocket.h"
#include <math.h>

//CommandStreamerSocket sock("localhost");
packet p;
#define TO_RADIAN 0.0174532925
void commandCallback(const ros::MessageEvent<geometry_msgs::Twist>& event)
{
  const std::string& publisher_name = event.getPublisherName();
  const ros::M_string& header = event.getConnectionHeader();
  ros::Time receipt_time = event.getReceiptTime();

  const boost::shared_ptr<geometry_msgs::Twist>& data = event.getMessage();
  std::cout << "received" << data->linear.x << std::endl;
  
  p.id = 42; //
  p.flags = 0;
  p.x = data->linear.x; // -10 < speed < 10
  p.y = 0; // unused
  p.turn = data->angular.z * TO_RADIAN; // -10 < orientation < 10 => 10 = 90°
  p.height = 5;
  //sock.sendPacket(p);
}

int main(int argc, char **argv)
{
  Suscriber<geometry_msgs::Twist> s("commandStreamer", argc, argv);

  s.init("cmd_vel", &commandCallback);
  return 0;
}
