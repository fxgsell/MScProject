/* Auto-generated by genmsg_cpp for file /opt/ros/groovy/share/nxt/nxt_msgs/msg/JointCommand.msg */
#ifndef NXT_MSGS_MESSAGE_JOINTCOMMAND_H
#define NXT_MSGS_MESSAGE_JOINTCOMMAND_H
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


namespace nxt_msgs
{
template <class ContainerAllocator>
struct JointCommand_ {
  typedef JointCommand_<ContainerAllocator> Type;

  JointCommand_()
  : name()
  , effort(0.0)
  {
  }

  JointCommand_(const ContainerAllocator& _alloc)
  : name(_alloc)
  , effort(0.0)
  {
  }

  typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _name_type;
  std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  name;

  typedef double _effort_type;
  double effort;


  typedef boost::shared_ptr< ::nxt_msgs::JointCommand_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::nxt_msgs::JointCommand_<ContainerAllocator>  const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;
}; // struct JointCommand
typedef  ::nxt_msgs::JointCommand_<std::allocator<void> > JointCommand;

typedef boost::shared_ptr< ::nxt_msgs::JointCommand> JointCommandPtr;
typedef boost::shared_ptr< ::nxt_msgs::JointCommand const> JointCommandConstPtr;


template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const  ::nxt_msgs::JointCommand_<ContainerAllocator> & v)
{
  ros::message_operations::Printer< ::nxt_msgs::JointCommand_<ContainerAllocator> >::stream(s, "", v);
  return s;}

} // namespace nxt_msgs

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator> struct IsMessage< ::nxt_msgs::JointCommand_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct IsMessage< ::nxt_msgs::JointCommand_<ContainerAllocator>  const> : public TrueType {};
template<class ContainerAllocator>
struct MD5Sum< ::nxt_msgs::JointCommand_<ContainerAllocator> > {
  static const char* value() 
  {
    return "9de38d5a93df00cd9489becfca3e874a";
  }

  static const char* value(const  ::nxt_msgs::JointCommand_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0x9de38d5a93df00cdULL;
  static const uint64_t static_value2 = 0x9489becfca3e874aULL;
};

template<class ContainerAllocator>
struct DataType< ::nxt_msgs::JointCommand_<ContainerAllocator> > {
  static const char* value() 
  {
    return "nxt_msgs/JointCommand";
  }

  static const char* value(const  ::nxt_msgs::JointCommand_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::nxt_msgs::JointCommand_<ContainerAllocator> > {
  static const char* value() 
  {
    return "string name\n\
float64 effort\n\
\n\
";
  }

  static const char* value(const  ::nxt_msgs::JointCommand_<ContainerAllocator> &) { return value(); } 
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::nxt_msgs::JointCommand_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.name);
    stream.next(m.effort);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct JointCommand_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::nxt_msgs::JointCommand_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const  ::nxt_msgs::JointCommand_<ContainerAllocator> & v) 
  {
    s << indent << "name: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.name);
    s << indent << "effort: ";
    Printer<double>::stream(s, indent + "  ", v.effort);
  }
};


} // namespace message_operations
} // namespace ros

#endif // NXT_MSGS_MESSAGE_JOINTCOMMAND_H

