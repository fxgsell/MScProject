#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/Imu.h>
#include <boost/assign/list_of.hpp>
#include <math.h>

int main(int argc, char** argv) {
  ros::init(argc, argv, "compass_publisher");

  ros::NodeHandle n;
  ros::Publisher imu_pub = n.advertise<sensor_msgs::Imu>("imu_data", 50);
  tf::TransformBroadcaster imu_broadcaster;

  double x = 0.0;
  double y = 0.0;
  double th = 0.0;

  double vx = 0.1;
  double vy = -0.1;
  double vth = 0.1;

  double compass_angle = 90;

  ros::Time current_time, last_time;
  current_time = ros::Time::now();
  last_time = ros::Time::now();

  ros::Rate r(1.0);
  double angle = 0;
  while(n.ok()){

    ros::spinOnce();               // check for incoming messages
    current_time = ros::Time::now();

    //compute odometry in a typical way given the velocities of the robot
    double dt = (current_time - last_time).toSec();
    double delta_x = (vx * cos(th) - vy * sin(th)) * dt;
    double delta_y = (vx * sin(th) + vy * cos(th)) * dt;
    double compass_yaw = atan (-compass_angle);

    double compass_angle = 90;
    x += delta_x;
    y += delta_y;

    //std::cout << "x=" << x << " y=" << y << std::endl;
    //since all odometry is 6DOF we'll need a quaternion created from yaw
    geometry_msgs::Quaternion imu_quat = tf::createQuaternionMsgFromYaw(compass_yaw);

    //first, we'll publish the transform over tf
    geometry_msgs::TransformStamped imu_trans;
    imu_trans.header.stamp = last_time;//current_time;
    imu_trans.header.frame_id = "imu_data";
    imu_trans.child_frame_id = "base_link";

    imu_trans.transform.translation.x = x;
    imu_trans.transform.translation.y = y;
    imu_trans.transform.translation.z = 0.0;
    imu_trans.transform.rotation = imu_quat;

    //send the transform
    imu_broadcaster.sendTransform(imu_trans);

    sensor_msgs::Imu imu;
    imu.header.stamp = current_time;
    imu.header.frame_id = "imu_data";

    angle += 0.001;
    imu.orientation.x = x;
    imu.orientation.y = y;
    imu.orientation.z = 0;
    imu.orientation.w = 0;
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
      std::cout << "published: [" << last_time << " " << current_time << " " << dt << "]" << std::endl;
    }
    else
      std::cout << "Waiting for suscriber" << std::endl;
    
    last_time = current_time;
    r.sleep();
  }
}