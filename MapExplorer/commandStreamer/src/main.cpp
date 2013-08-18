#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <ros/message_event.h>
#include "protocol.h"
#include "Suscriber.hpp"
#include "commandStreamerSocket.h"
#include <math.h>

CommandStreamerSocket *sock;
int speedRate = 10;
const char *ip = "192.168.120.102";

unsigned int mxtime = 100; // update time

packet p;
#define TO_DEGRE 0.0174532925
void commandCallback(const ros::MessageEvent<geometry_msgs::Twist>& event)
{
  static int cpt = 0;
  cpt++;
  if (cpt % 500 == 0)
    return;
  const boost::shared_ptr<geometry_msgs::Twist>& data = event.getMessage();

  std::cout << (float)data->linear.x << "  " 
	    << (float)data->angular.z << "(rad)  " << std::endl;
  p.id = 42; //
  p.flags = 0;
  p.x = data->linear.x * speedRate; // -10 < speed < 10
  p.y = 0; // unused
  p.turn = (data->angular.z / TO_DEGRE) / 9; // -10 < orientation < 10 => 10 = 90°
  if (data->linear.x < 1 && data->linear.x > 0)
    p.x = 1;
  else if (data->linear.x > 1 && data->linear.x < 0)
    p.x = -1;
  if (data->linear.z < 6)
    data->linear.z = 6;
  else if (data->linear.z < -6)
    data->linear.z = -6;

  p.height = 20;// ~maximum value
  std::cout << "SEND PACKET: [x=" << p.x << ";turn=" << p.turn << ";height=" << p.height << "]" << std::endl;
  sock->sendPacket(p);
}

int main(int argc, char **argv)
{
  if (argc > 1)
    ip = argv[1];
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
