-- Tested on Ubuntu 12.04 LTS Precise Pangolin --


## Import the project folder if you haven't already:

sudo git clone git@github.com:zukoo/MScProject.git

## Install ROS groovy:

Setup the sources.list:

sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu quantal main" > /etc/apt/sources.list.d/ros-latest.list'
wget http://packages.ros.org/ros.key -O - | sudo apt-key add -

sudo apt-get update

sudo apt-get install ros-groovy-desktop-full

sudo rosdep init

rosdep update

echo "source /opt/ros/groovy/setup.bash" >> ~/.bashrc
source ~/.bashrc

Roinstall utility:

sudo apt-get install python-setuptools
sudo apt-get install python-rosinstall python-rosdep
sudo rosdep init
rosdep update

## For networking with ROS:

To setup an ethernet network type:

sudo ifconfig eth0:1 192.168.16.1 up

To enable NAT type:

sudo   iptables -t nat -A POSTROUTING -s 192.168.16.0/24 -o eth0 -j MASQUERADE
					 /\ ip address of the robot's computer.

For either ethernet or wi-fi networks type:

sudo apt-get install chrony

To find hostnames type 

hostname
(my_robot) returned.

Then on the robot type 

export ROS_HOSTNAME=my_robot.local
roscore

Any subsequent nodes will be attached to the master 'core' node.

On the desktop type

export ROS_HOSTNAME=my_desktop.local
export ROS_MASTER_URI=http://my_robot.local:11311

To check time snychoronisation, run the ntpdate command to synchronise the desktop with the robot.

sudo ntpdate -b my_robot.local

Now on the desktop:

rostopic list 

Should output /rosout and /rosout_agg.

## rbx1 install (for simulating a robot model)-

Installing the prerequisites: 

sudo apt-get install ros-groovy-turtlebot* ros-groovy-control \
ros-groovy-openni-camera ros-groovy-openni-launch \
ros-groovy-openni-tracker ros-groovy-laser-drivers \
ros-groovy-audio-common ros-groovy-joystick-drivers \
ros-groovy-orocos-kinematics-dynamics \
ros-groovy-dynamixel-motor gstreamer0.10-pocketsphinx \
python-setuptools ros-groovy-opencv2 ros-groovy-vision-opencv \
ros-groovy-depthimage-to-laserscan ros-groovy-arbotix \
git subversion mercurial

sudo cp -r ~/MScProject/MapExplorer/rbx1 /opt/ros/groovy/share/
cd /opt/ros/groovy/share/rbx1
rosmake
rospack profile

## Hector_exploration install

cd /opt/ros/groovy/share
sudo -s
git clone https://github.com/tu-darmstadt-ros-pkg/hector_navigation.git
cd hector_navigation
rosdep install hector_navigation
rosmake

## ccny_rgbd install

cd /opt/ros/groovy/share
sudo git clone  https://github.com/ccny-ros-pkg/ccny_rgbd_tools.git

Prerequisites:

rosdep install ccny_rbgd_tools
sudo apt-get remove ros-groovy-libg2o
sudo apt-get install ros-groovy-octomap*
sudo apt-get install ros-groovy-openni*
sudo cp ~/MScProject/MapExplorer/ccny/visual_odometry.cpp /opt/ros/groovy/share/ccny_rgbd_tools/ccny_rgbd/src/apps/
cd /opt/ros/groovy/share/ccny_rgbd_tools 
rosmake

## Robot_pose_ekf 

sudo cp -r ~/MScProject/MapExplorer/robot_pose_ekf /opt/ros/groovy/stacks/

cd /opt/ros/groovy/stacks/robot_pose_ekf

rosmake

This replaces the robot_pose_ekf folder and launch file.


## NXT_ros install (for compass orientation).

cd /opt/ros/groovy/share
sudo -s
sudo cp -r ~/MScProject/MapExplorer/nxt .
cd nxt
rosdep install nxt
rosmake nxt

Warning: This launcher can be temperamental, if the launcher fails for whatever reason: first make sure the nxt brick is turned on, secondly recopy and remake the folder and it will work.


## Make and compile commandStreamer for the hexapod.

sudo cp -r /MScProject/commandStreamer /opt/ros/groovy/share/
cd /opt/ros/groovy/share/commandStreamer
sudo -s
cmake .
make

## To toggle holonomic movement of the robot:

roscd hector_path_follower
cd src
sudo nano hector_path_follower.cpp
change  the parameter "holonomic" to true
cd ..
rosmake

## To change the gmapping settings (for example update frequency)

roscd rbx1_nav
cd launch
sudo nano gmapping.launch
change map_update_interval to a lower number for a higher update rate 
(higher computation rate and may create a less accurate map, depending on the accuracy of your odometry, the less accurate the odometry the less accurate the map will be with more frequent updates)

## To change the minimium collision distance

roscd rbx1_description
sudo nano urdf/turtlebot_body.urdf.xacro

Change the following radius if you wish to optimise getting closer to objects or have
a lower minimum distance on your laser scanner. 

(Search for the string "Change" to go direct to this object).

Change the collision of base_link to cylinder radius ="x"
The default is the minimum kinect scanning distance/2 + the 
offset of the kinect on the plate_2_link  (85cm + 16cm = 101cm) = (95cm effective as 
hector_exploration_planner sets the default min obstacle distance at 10cm and the
model places the kinect 
at the back of the robot).


## To change hector_exploration settings

roscd hector_exploration_planner
cd cfg
sudo nano ExplorationPlanner.cfg
change min_obstacle_dist to change the mimimum distance the robot will walk towards an object -
default = 10cm. Minimise this to optimise obstacle weaving.
change min_frontier_size smaller to explore smaller spaces or larger to explore wider spaces.
change dist_for_goal_reached to larger if the robot does not believe it has reached it's goal when it has.

## General manual launch sequence:

roslaunch ccny_openni_launch openni.launch publish_cloud:=true
roslaunch ccny_rgbd visual_odometry.launch 
roslaunch rbx1_nav gmapping.launch
rosrun depthimage_to_laserscan depthimage_to_laserscan image:=/camera/depth/image_raw
roslaunch hector_exploration_node exploration_planner.launch 
roslaunch rbx1_bringup turtlebot_minimal_create.launch
rosrun rviz rviz -d `rospack find rbx1_nav`/gmapping.rviz
roslaunch robot_pose_ekf robot_pose_ekf.launch
roslaunch compass compass.launch 
rosrun hector_exploration_controller simple_exploration_controller 
rosrun hector_path_follower hector_path_follower_node
roscd /opt/ros/groovy/share/commandStreamer/devel/lib/commandStreamer 
./commandStreamer 192.168.120.102
		  /\ ip of the robot


## launcher
Instead of using the manual launching sequence, you can use the launcher, which will launch step by step the whole sequence.
## how to build it
cd MapExplorer/LaunchMapExplorer/src/launcher/
cmake .
make
cd devel/lib/launcher/
/!\ Create your own launcher.msc or paste an existing file in the same directory
./launcher



## How to use it
cd MapExplorer/LaunchMapExplorer
./launcher
If you want to update the sequence, you have to edit the launcher.msc file, which contain the sequence to Json format. The launcher parses this file and executes each command in it.
Command example:
{"cmd":"roslaunch ccny_rgbd visual_odometry.launch",
"topicName":"/vo",
"topicType":"nav_msgs::Odometry"}, ## Remove the comma for the last sequence


Json files contain a collection of pair name/value, in the ‘launcher.msc’ file we have:
-cmd: which is the command that have to be launch
-topicName: some ROS packages have to be launch only when the previous package has been launched and has started to publish on a specific topic. You can stop the launch sequence while nothing is publish on the specify topicName or write “none” to go directly to the next sequence.
-topicType: If you have specified a topicName you have to specify the type of message publish on this topic.


If one or both topicName and TopicType are set to “none”, the launcher goes directly to the next sequence.




## commandStreamer
commandStreamer is a program use to convert and stream the command create by ROS (cmd_vel topic) to command the robot.
## how to build & use the commandStreamer
cd MapExplorer/commandStreamer
cmake .
make
cd /devel/lib/commandStreamer
./commandStreamer [ip]	## ip defaut value 192.168.120.102







