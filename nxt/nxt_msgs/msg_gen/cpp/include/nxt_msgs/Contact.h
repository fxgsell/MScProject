/* Auto-generated by genmsg_cpp for file /opt/ros/groovy/share/nxt/nxt_msgs/msg/Contact.msg */
#ifndef NXT_MSGS_MESSAGE_CONTACT_H
#define NXT_MSGS_MESSAGE_CONTACT_H
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

namespace nxt_msgs
{
template <class ContainerAllocator>
struct Contact_ {
  typedef Contact_<ContainerAllocator> Type;

  Contact_()
  : header()
  , contact(false)
  {
  }

  Contact_(const ContainerAllocator& _alloc)
  : header(_alloc)
  , contact(false)
  {
  }

  typedef  ::std_msgs::Header_<ContainerAllocator>  _header_type;
   ::std_msgs::Header_<ContainerAllocator>  header;

  typedef uint8_t _contact_type;
  uint8_t contact;


  typedef boost::shared_ptr< ::nxt_msgs::Contact_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::nxt_msgs::Contact_<ContainerAllocator>  const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;
}; // struct Contact
typedef  ::nxt_msgs::Contact_<std::allocator<void> > Contact;

typedef boost::shared_ptr< ::nxt_msgs::Contact> ContactPtr;
typedef boost::shared_ptr< ::nxt_msgs::Contact const> ContactConstPtr;


template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const  ::nxt_msgs::Contact_<ContainerAllocator> & v)
{
  ros::message_operations::Printer< ::nxt_msgs::Contact_<ContainerAllocator> >::stream(s, "", v);
  return s;}

} // namespace nxt_msgs

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator> struct IsMessage< ::nxt_msgs::Contact_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct IsMessage< ::nxt_msgs::Contact_<ContainerAllocator>  const> : public TrueType {};
template<class ContainerAllocator>
struct MD5Sum< ::nxt_msgs::Contact_<ContainerAllocator> > {
  static const char* value() 
  {
    return "771b237034839b48d5de401dc68aaf43";
  }

  static const char* value(const  ::nxt_msgs::Contact_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0x771b237034839b48ULL;
  static const uint64_t static_value2 = 0xd5de401dc68aaf43ULL;
};

template<class ContainerAllocator>
struct DataType< ::nxt_msgs::Contact_<ContainerAllocator> > {
  static const char* value() 
  {
    return "nxt_msgs/Contact";
  }

  static const char* value(const  ::nxt_msgs::Contact_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::nxt_msgs::Contact_<ContainerAllocator> > {
  static const char* value() 
  {
    return "Header header\n\
bool contact\n\
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
";
  }

  static const char* value(const  ::nxt_msgs::Contact_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator> struct HasHeader< ::nxt_msgs::Contact_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct HasHeader< const ::nxt_msgs::Contact_<ContainerAllocator> > : public TrueType {};
} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::nxt_msgs::Contact_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.header);
    stream.next(m.contact);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct Contact_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::nxt_msgs::Contact_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const  ::nxt_msgs::Contact_<ContainerAllocator> & v) 
  {
    s << indent << "header: ";
s << std::endl;
    Printer< ::std_msgs::Header_<ContainerAllocator> >::stream(s, indent + "  ", v.header);
    s << indent << "contact: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.contact);
  }
};


} // namespace message_operations
} // namespace ros

#endif // NXT_MSGS_MESSAGE_CONTACT_H

