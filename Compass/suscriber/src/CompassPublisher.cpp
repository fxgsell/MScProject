
#include "CompassPublisher.h"

CompassPublisher::CompassPublisher(int argc, char**argv, std::string const &name) {
  ros::init(argc, argv, name);
  n = new ros::NodeHandle();
  imu_pub = n->advertise<sensor_msgs::Imu>("imu_data", 50);
  current_time = ros::Time::now();
  last_time = ros::Time::now();
}

CompassPublisher::~CompassPublisher() {
}

void CompassPublisher::publish(double w, double x, double y, double z) {
  sensor_msgs::Imu imu;
  imu.header.stamp = current_time;
  imu.header.frame_id = "imu_data";
  
  imu.orientation.x = x;
  imu.orientation.y = y;
  imu.orientation.z = z;
  imu.orientation.w = w;
  imu.orientation_covariance = boost::assign::list_of
    (1000) (0) (0)
    (0) (1000) (0)
    (0) (0) (1000);
  
  imu.angular_velocity.x = 0;
  imu.angular_velocity.y = 0;
  imu.angular_velocity.z = 0;
  imu.angular_velocity_covariance = boost::assign::list_of
    (-1) (-1)   (-1)
    (-1) (-1)   (-1)
    (-1) (-1)   (-1);
  
  imu.linear_acceleration.x = 0;
  imu.linear_acceleration.y = 0;
  imu.linear_acceleration.z = 0;
  imu.linear_acceleration_covariance = boost::assign::list_of
    (-1) (-1)   (-1) 
    (-1) (-1)   (-1) 
    (-1) (-1)   (-1);
  
  //publish the message
  if (imu_pub.getNumSubscribers() > 0) {
    imu_pub.publish(imu);
    std::cout << "published: [" << w << ";" << x << ";" 
	      << y << ";" << z << "]" << std::endl;
  }
  else
    std::cout << "Waiting for suscriber" << std::endl;
}
