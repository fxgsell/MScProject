/* Auto-generated by genmsg_cpp for file /opt/ros/groovy/share/nxt/nxt_msgs/msg/Compass.msg */
#ifndef NXT_MSGS_MESSAGE_COMPASS_H
#define NXT_MSGS_MESSAGE_COMPASS_H
#include <string>
#include <vector>
#include <map>
#include <ostream>
#include "ros/serialization.h"
#include "ros/builtin_message_traits.h"
#include "ros/message_operations.h"
#include "ros/time.h"

#include "ros/macros.h"

#include "ros/assert.h"

#include "std_msgs/Header.h"
#include "geometry_msgs/Quaternion.h"

namespace nxt_msgs
{
template <class ContainerAllocator>
struct Compass_ {
  typedef Compass_<ContainerAllocator> Type;

  Compass_()
  : header()
  , orientation()
  {
  }

  Compass_(const ContainerAllocator& _alloc)
  : header(_alloc)
  , orientation(_alloc)
  {
  }

  typedef  ::std_msgs::Header_<ContainerAllocator>  _header_type;
   ::std_msgs::Header_<ContainerAllocator>  header;

  typedef  ::geometry_msgs::Quaternion_<ContainerAllocator>  _orientation_type;
   ::geometry_msgs::Quaternion_<ContainerAllocator>  orientation;


  typedef boost::shared_ptr< ::nxt_msgs::Compass_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::nxt_msgs::Compass_<ContainerAllocator>  const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;
}; // struct Compass
typedef  ::nxt_msgs::Compass_<std::allocator<void> > Compass;

typedef boost::shared_ptr< ::nxt_msgs::Compass> CompassPtr;
typedef boost::shared_ptr< ::nxt_msgs::Compass const> CompassConstPtr;


template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const  ::nxt_msgs::Compass_<ContainerAllocator> & v)
{
  ros::message_operations::Printer< ::nxt_msgs::Compass_<ContainerAllocator> >::stream(s, "", v);
  return s;}

} // namespace nxt_msgs

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator> struct IsMessage< ::nxt_msgs::Compass_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct IsMessage< ::nxt_msgs::Compass_<ContainerAllocator>  const> : public TrueType {};
template<class ContainerAllocator>
struct MD5Sum< ::nxt_msgs::Compass_<ContainerAllocator> > {
  static const char* value() 
  {
    return "1d6099487ad91df0521eab1e92146a95";
  }

  static const char* value(const  ::nxt_msgs::Compass_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0x1d6099487ad91df0ULL;
  static const uint64_t static_value2 = 0x521eab1e92146a95ULL;
};

template<class ContainerAllocator>
struct DataType< ::nxt_msgs::Compass_<ContainerAllocator> > {
  static const char* value() 
  {
    return "nxt_msgs/Compass";
  }

  static const char* value(const  ::nxt_msgs::Compass_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::nxt_msgs::Compass_<ContainerAllocator> > {
  static const char* value() 
  {
    return "Header header\n\
geometry_msgs/Quaternion orientation\n\
\n\
================================================================================\n\
MSG: std_msgs/Header\n\
# Standard metadata for higher-level stamped data types.\n\
# This is generally used to communicate timestamped data \n\
# in a particular coordinate frame.\n\
# \n\
# sequence ID: consecutively increasing ID \n\
uint32 seq\n\
#Two-integer timestamp that is expressed as:\n\
# * stamp.secs: seconds (stamp_secs) since epoch\n\
# * stamp.nsecs: nanoseconds since stamp_secs\n\
# time-handling sugar is provided by the client library\n\
time stamp\n\
#Frame this data is associated with\n\
# 0: no frame\n\
# 1: global frame\n\
string frame_id\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Quaternion\n\
# This represents an orientation in free space in quaternion form.\n\
\n\
float64 x\n\
float64 y\n\
float64 z\n\
float64 w\n\
\n\
";
  }

  static const char* value(const  ::nxt_msgs::Compass_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator> struct HasHeader< ::nxt_msgs::Compass_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct HasHeader< const ::nxt_msgs::Compass_<ContainerAllocator> > : public TrueType {};
} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::nxt_msgs::Compass_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.header);
    stream.next(m.orientation);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct Compass_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::nxt_msgs::Compass_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const  ::nxt_msgs::Compass_<ContainerAllocator> & v) 
  {
    s << indent << "header: ";
s << std::endl;
    Printer< ::std_msgs::Header_<ContainerAllocator> >::stream(s, indent + "  ", v.header);
    s << indent << "orientation: ";
s << std::endl;
    Printer< ::geometry_msgs::Quaternion_<ContainerAllocator> >::stream(s, indent + "  ", v.orientation);
  }
};


} // namespace message_operations
} // namespace ros

#endif // NXT_MSGS_MESSAGE_COMPASS_H

