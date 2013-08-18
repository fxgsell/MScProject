FILE(REMOVE_RECURSE
  "../msg_gen"
  "../msg_gen"
  "../src/nxt_msgs/msg"
  "CMakeFiles/ROSBUILD_genmsg_py"
  "../src/nxt_msgs/msg/__init__.py"
  "../src/nxt_msgs/msg/_Range.py"
  "../src/nxt_msgs/msg/_JointCommand.py"
  "../src/nxt_msgs/msg/_Accelerometer.py"
  "../src/nxt_msgs/msg/_Contact.py"
  "../src/nxt_msgs/msg/_Color.py"
  "../src/nxt_msgs/msg/_Gyro.py"
  "../src/nxt_msgs/msg/_Compass.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
