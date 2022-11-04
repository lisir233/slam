cmake_minimum_required(VERSION 2.8.3)
project(pointcloud_deal)

add_compile_options(-std=c++14)


find_package(catkin REQUIRED COMPONENTS
pcl_ros
roscpp
sensor_msgs
OpenCV
)

find_package(PCL 1.7 REQUIRED)

catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS roscpp sensor_msgs pcl_ros
)

include_directories(
 include
 ${catkin_INCLUDE_DIRS}
 ${OpenCV_INCLUDE_DIRS}
)
link_directories(${PCL_LIBRARY_DIRS})

add_executable(${PROJECT_NAME}_node src/pointcloud_deal_node.cpp src/pointcloud_deal_core.cpp)


target_link_libraries(${PROJECT_NAME}_node
  ${catkin_LIBRARIES}
  ${PCL_LIBRARIES}
  ${OpenCV_LIBRARIES}
)
