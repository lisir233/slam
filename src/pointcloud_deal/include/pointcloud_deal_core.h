#pragma once

#include <ros/ros.h>

#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_types.h>
#include <pcl/conversions.h>
#include <pcl_ros/transforms.h>

#include <pcl/filters/extract_indices.h>
#include <pcl/filters/passthrough.h>
#include <pcl/ModelCoefficients.h>       
#include <pcl/filters/project_inliers.h> 

#include <sensor_msgs/PointCloud2.h>
#include <pcl/filters/voxel_grid.h>//体素滤波
#include <pcl/features/normal_3d.h>//法线估计
#include <pcl/segmentation/conditional_euclidean_clustering.h>//条件欧式聚类
#include <pcl/segmentation/extract_clusters.h>

#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>


#define CLU_TOLERANCE 0.08
#define CLIP_HEIGHT 1 //截取掉高于雷达自身1米的点
#define MIN_DISTANCE 0.3
#define RADIAL_DIVIDER_ANGLE 0.18
#define SENSOR_HEIGHT 1.5 //雷达自身高度

#define concentric_divider_distance_ 0.01 //0.1 meters default
#define min_height_threshold_ 0.05
#define local_max_slope_ 8   //max slope of the ground between points, degree
#define general_max_slope_ 5 //max slope of the ground in entire point cloud, degree
#define reclass_distance_threshold_ 0.2

class PointcloudDealCore
{

private:
  ros::Subscriber sub_point_cloud_;

  ros::Publisher pub_curvatured_ptr_;

  void point_cb(const sensor_msgs::PointCloud2ConstPtr &in_cloud);
  void publish_cloud(const ros::Publisher &in_publisher,
                                  const pcl::PointCloud<pcl::PointXYZI>::Ptr in_cloud_to_publish_ptr,
                                  const std_msgs::Header &in_header);
  void projection_3D_to_2D(pcl::PointCloud<pcl::PointXYZI>::Ptr in,
    pcl::PointCloud<pcl::PointXYZI>::Ptr out);
  void euclideanclustering(double ClusterTolerance,  pcl::PointCloud<pcl::PointXYZI>::Ptr in,
                                   std::vector<pcl::PointIndices>* cluster_indices);
  void delete_small_cluster(double length_threshold,  pcl::PointCloud<pcl::PointXYZI>::Ptr source_3d, pcl::PointCloud<pcl::PointXYZI>::Ptr source_2d,
                                  std::vector<pcl::PointIndices>* cluster_indices,pcl::PointCloud<pcl::PointXYZI>::Ptr out);
public:
  PointcloudDealCore(ros::NodeHandle &nh);
  ~PointcloudDealCore();
  void Spin();
};