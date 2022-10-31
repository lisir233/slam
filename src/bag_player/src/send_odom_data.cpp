#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include "tf/transform_listener.h"

#include <serial/serial.h>

#include "math.h"
#include "string.h"
//串口类
serial::Serial ser;
 
#define sBUFFERSIZE 1000 // send buffer size串口发送缓存长度
#define rBUFFERSIZE 1000 // receive buffer size 串口接收缓存长度
char s_buffer[sBUFFERSIZE]; //发送缓存
unsigned char r_buffer[rBUFFERSIZE]; //接收缓存


void OdomCallback(const nav_msgs::Odometry::ConstPtr &msg)
{
    char tx_buffer[20];
    double x, y, z;
    double roll, pitch, yaw;
    x = msg->pose.pose.position.x;
    y = msg->pose.pose.position.y;
    z = msg->pose.pose.position.z;
    tf::Quaternion quat;                                     //定义一个四元数
    tf::quaternionMsgToTF(msg->pose.pose.orientation, quat); //取出方向存储于四元数
    tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);

    sprintf(tx_buffer,"odom %.3f\r\n",sqrt(x*x+y*y));
    ROS_INFO("%s",tx_buffer);
    if(ser.isOpen())
    {
        ser.write((uint8_t*)s_buffer, strlen(s_buffer));
    }
    //ROS_INFO("Odom: %f, %f, %f, %f, %f, %f", x, y, z, roll, pitch, yaw);
}
 
int main(int argc, char **argv)
{
    ros::init(argc, argv, "listener");
    ros::NodeHandle node;
    ros::Subscriber subOdom = node.subscribe("raw_odom", 1000, OdomCallback);

//打开串口
  try {
    ser.setPort("/dev/ttyUSB1");
    ser.setBaudrate(9600);
    serial::Timeout to = serial::Timeout::simpleTimeout(1000);
    ser.setTimeout(to);
    ser.open();
  } catch (serial::IOException &e) {
    ROS_ERROR_STREAM("Unable to open port ");
  }
 
  if (ser.isOpen()) {
    ROS_INFO_STREAM("Serial Port initialized");
  } else {

  }
    ROS_INFO_STREAM("Init done");
    ros::spin();
    return 0;
}