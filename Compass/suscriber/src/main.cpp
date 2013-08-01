#include "ros/ros.h"
#include "geometry_msgs/Quaternion.h"
#include <ros/message_event.h>
#include "CompassPublisher.h"

CompassPublisher *publisher;

void chatterCallback(const ros::MessageEvent<geometry_msgs::Quaternion>& event)
{
  const std::string& publisher_name = event.getPublisherName();
  const ros::M_string& header = event.getConnectionHeader();
  ros::Time receipt_time = event.getReceiptTime();

  const boost::shared_ptr<geometry_msgs::Quaternion>& data = event.getMessage();
  std::cout << "received" << data->x << std::endl;
  publisher->publish(data->w, data->x, data->y, data->z);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  publisher = new CompassPublisher(argc, argv);

  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe<geometry_msgs::Quaternion>("chatter", 1000, chatterCallback);

  ros::spin();

  return 0;
}
