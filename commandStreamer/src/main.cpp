
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <ros/message_event.h>
#include "protocol.h"
#include "Suscriber.hpp"
#include "commandStreamerSocket.h"
#include <math.h>

CommandStreamerSocket *sock;
int speedRate = 20;
const char *ip = "192.168.120.102";

packet p;
#define TO_RADIAN 0.0174532925
void commandCallback(const ros::MessageEvent<geometry_msgs::Twist>& event)
{
  const boost::shared_ptr<geometry_msgs::Twist>& data = event.getMessage();

  std::cout << (float)data->linear.x << "  " 
	    << (float)data->angular.z << "(rad)  " << std::endl;
  p.id = 42; //
  p.flags = 0;
  p.x = data->linear.x * 20; // -10 < speed < 10
  if (data->linear.x < 1 && data->linear.x > 0)
    p.x = 1;
  else if (data->linear.x > 1 && data->linear.x < 0)
    p.x = -1;
  p.y = 0; // unused
  p.turn = data->angular.z / TO_RADIAN; // -10 < orientation < 10 => 10 = 90°
  p.height = 5;
  std::cout << "SEND PACKET: [" << p.x << ";" << p.turn << "]" << std::endl;
  sock->sendPacket(p);
}

int main(int argc, char **argv)
{
  if (argc > 1) {
    ip = argv[1];
    if (argc == 3)
      speedRate = atoi(argv[2]);
  }
  std::cout << "Start: IP=" << ip << " speedRate=" << speedRate << std::endl;  
  ros::init(argc, argv, "cmd_vel");
  sock = new CommandStreamerSocket(ip);

  Suscriber<geometry_msgs::Twist> s("cmd_vel", commandCallback);
  ros::spin();
  /*
  ros::init(argc, argv, "cmd_vel");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("cmd_vel", 1000, commandCallback);
  if (!sub)
    std::cout << "init suscriber: FAIL" << std::endl;
  else
    std::cout << "init suscriber: OK" << std::endl;  ros::spin();
  */
  return 0;
}
