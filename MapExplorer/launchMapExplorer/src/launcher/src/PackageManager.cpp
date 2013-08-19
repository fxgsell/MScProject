 
#include "PackageManager.h"
#include "Subscriber.hpp"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
bool           packageLaunched;
ProcessManager process;
bool first = true;

void PackageManager::killThemAll() {
 process.killThemAll();
}


template<typename T>
void run(std::string const &cmd, std::string const &topic, void (*cb)(const ros::MessageEvent<T>&)) {
  char **dummy;
  int a = 0;
  process.launchNewPackage(cmd);
  if (first) {
    first = false;
    ros::init(a, dummy, "launcher");
    sleep(1);
  }
  Suscriber<T> s(topic, cb);
  while(!packageLaunched) {
    ros::spinOnce();
    usleep(10);
  }
}

void PackageManager::launchPackage(std::string const &cmd, std::string const &topic, std::string const &type) {
  packageLaunched = false;
  if ((topic != "none" && type != "none") 
      && _handler.find(type) != _handler.end()) {
    (*_handler.find(type)->second)(cmd, topic);
  }
  else {
    if (_handler.find(type) != _handler.end())
      std::cout << "[WARNING]: Type wasn't found: " << type << std::endl;
    process.launchNewPackage(cmd); 
  }
}

/* openni.launch */

void sensor_msgs_Image_callback(const ros::MessageEvent<sensor_msgs::Image>&) { 
  packageLaunched = true; 
}

void sensor_msgs_Image_handler(std::string const &cmd, std::string const &topic) {
  run<sensor_msgs::Image>(cmd, topic, sensor_msgs_Image_callback);
}

/*  */
void nav_msgs_Odometry_callback(const ros::MessageEvent<nav_msgs::Odometry>&) 
{ packageLaunched = true; }

void nav_msgs_Odometry_handler(std::string const &cmd, std::string const &topic) {
  run<nav_msgs::Odometry>(cmd, topic, nav_msgs_Odometry_callback);
}

/* CommandStreamer */
void geometry_msgs_Twist_callback(const ros::MessageEvent<geometry_msgs::Twist>&) 
{ packageLaunched = true; }

void geometry_msgs_Twist_handler(std::string const &cmd, std::string const &topic) {
  run<geometry_msgs::Twist>(cmd, topic, geometry_msgs_Twist_callback);
}

/* gmapping */
void nav_msgs_OccupancyGrid_callback(const ros::MessageEvent<nav_msgs::OccupancyGrid>&) 
{ packageLaunched = true; }

void nav_msgs_OccupancyGrid_handler(std::string const &cmd, std::string const &topic) {
  run<nav_msgs::OccupancyGrid>(cmd, topic, nav_msgs_OccupancyGrid_callback);
}

/* robot_pose_ekf */
void geometry_msgs_PoseWithCovarianceStamped_callback(const ros::MessageEvent<geometry_msgs::PoseWithCovarianceStamped>&) {
  packageLaunched = true; 
}

void geometry_msgs_PoseWithCovarianceStamped_handler(std::string const &cmd, std::string const &topic) {
   run<geometry_msgs::PoseWithCovarianceStamped>(cmd, topic, geometry_msgs_PoseWithCovarianceStamped_callback);
}

PackageManager::PackageManager() {
  _handler["sensor_msgs::Image"] = sensor_msgs_Image_handler;
  _handler["geometry_msgs::Twist"] = geometry_msgs_Twist_handler;
  _handler["nav_msgs::Odometry"] = nav_msgs_Odometry_handler;
  _handler["nav_msgs::OccupancyGrid"] = nav_msgs_OccupancyGrid_handler;
  _handler["geometry_msgs::PoseWithCovarianceStamped"] = geometry_msgs_PoseWithCovarianceStamped_handler;
  packageLaunched = false;
}

PackageManager::~PackageManager() {
}

