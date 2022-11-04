#include "pointcloud_deal_core.h"

typedef pcl::PointXYZI PointTypeIO;
typedef pcl::PointXYZINormal PointTypeFull;

#define MIN_CLUSTER_SIZE 50
#define MAX_CLUSTER_SIZE 100000

PointcloudDealCore::PointcloudDealCore(ros::NodeHandle &nh)
{
    sub_point_cloud_ = nh.subscribe("/filtered_points_no_ground", 10, &PointcloudDealCore::point_cb, this);

    pub_curvatured_ptr_ = nh.advertise<sensor_msgs::PointCloud2>("/filtered_remove_powerintensity", 10);

    ros::spin();
}

PointcloudDealCore::~PointcloudDealCore() {}

void PointcloudDealCore::Spin()
{
}
void PointcloudDealCore::publish_cloud(const ros::Publisher &in_publisher,
                                const pcl::PointCloud<pcl::PointXYZI>::Ptr in_cloud_to_publish_ptr,
                                const std_msgs::Header &in_header)
{
    sensor_msgs::PointCloud2 cloud_msg;
    pcl::toROSMsg(*in_cloud_to_publish_ptr, cloud_msg);
    cloud_msg.header = in_header;
    in_publisher.publish(cloud_msg);
}

void PointcloudDealCore::projection_3D_to_2D_pt(double power_threshold,  pcl::PointCloud<pcl::PointXYZI>::Ptr in,
                                   pcl::PointCloud<pcl::PointXYZI>::Ptr out)
{
    pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients());
    coefficients->values.resize(4);
    coefficients->values[0] = 0;
    coefficients->values[1] = 0;
    coefficients->values[2] = 1;
    coefficients->values[3] = 0;

// Create the filtering object
    pcl::ProjectInliers<pcl::PointXYZI> proj;
    proj.setModelType(pcl::SACMODEL_PLANE);   //创建投影类型，投影到平面上
    proj.setInputCloud(in);
    proj.setModelCoefficients(coefficients);
    proj.filter(*out);

} 
// void PointcloudDealCore::euclideanclustering_pt(double power_threshold,  pcl::PointCloud<pcl::PointXYZI>::Ptr in,
//                                    pcl::PointCloud<pcl::PointXYZI>::Ptr out)
// {
// 	vector<pcl::PointIndices> cluster_indices;
//     //pcl::IndicesClustersPtr clusters(new pcl::IndicesClusters), small_clusters(new pcl::IndicesClusters), large_clusters(new pcl::IndicesClusters);//创建索引
// 	//pcl::PointCloud<PointTypeFull>::Ptr cloud_with_normals(new pcl::PointCloud<PointTypeFull>);//创建法线估计
//    // pcl::PointCloud<PointTypeIO>::Ptr cloud_in(new pcl::PointCloud<PointTypeIO>), out(new pcl::PointCloud<PointTypeIO>);//创建cloud_in和out指针
// 	//pcl::search::KdTree<PointTypeIO>::Ptr search_tree(new pcl::search::KdTree<PointTypeIO>);//创建搜索方式
// 	pcl::search::KdTree<pcl::PointXYZI>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZI>);
//     // pcl::VoxelGrid<PointTypeIO> vg;//创建体素滤波对象
// 	// vg.setInputCloud(in);//设置输入点云
// 	// vg.setLeafSize(0.1, 0.1, 0.1);//设置叶尺寸
// 	// vg.setDownsampleAllData(true);//设置是否对所有点进行体素滤波
// 	// vg.filter(*out);//执行滤波，并将结果保存在out中

//     // Set up a Normal Estimation class and merge data in cloud_with_normals
// 	//建立法线估计类，并将数据合并到cloud_with_normals
// 	// pcl::copyPointCloud(*in, *cloud_with_normals);//复制点云
// 	// pcl::NormalEstimation<PointTypeIO, PointTypeFull> ne;//创建法线估计对象
// 	// ne.setInputCloud(in);//设置输入点云
// 	// ne.setSearchMethod(search_tree);//设置所搜方式
// 	// ne.setRadiusSearch(300.0);//设置搜索半径大小
// 	// ne.compute(*cloud_with_normals);//惊醒法线计算
 
// 	// // Set up a Conditional Euclidean Clustering class
// 	// //建立条件欧式聚类对象
// 	// pcl::ConditionalEuclideanClustering<PointTypeFull> cec(true);//创建条件聚类分割对象，并进行初始化
// 	// cec.setInputCloud(cloud_with_normals);//设置输入点云
// 	// cec.setConditionFunction(&customRegionGrowing);//设置搜索函数
// 	// cec.setClusterTolerance(500.0);//设置聚类参考点的所搜距离
// 	// cec.setMinClusterSize(cloud_with_normals->points.size() / 1000);//设置过小聚类的标准
// 	// cec.setMaxClusterSize(cloud_with_normals->points.size() / 5);//设置过大聚类的标准
// 	// cec.segment(*clusters);//获取聚类的结果，分割结果保存在点云索引的向量中
// 	// cec.getRemovedClusters(small_clusters, large_clusters);//获取无效尺寸的聚类

// 		pcl::EuclideanClusterExtraction<pcl::PointXYZI> euclid;
// 		euclid.setInputCloud(in);
// 		euclid.setClusterTolerance(0.05);
// 		euclid.setMinClusterSize(MIN_CLUSTER_SIZE);
// 		euclid.setMaxClusterSize(MAX_CLUSTER_SIZE);
// 		euclid.setSearchMethod(tree);
// 		euclid.extract(cluster_indices);
//     // Using the intensity channel for lazy visualization of the output
// 	//使用强度通道对输出进行延迟可视化
// 	// for (int i = 0; i < small_clusters->size(); ++i)
// 	// 	for (int j = 0; j < (*small_clusters)[i].indices.size(); ++j)
// 	// 		out->points[(*small_clusters)[i].indices[j]].intensity = -2.0;
// 	// for (int i = 0; i < large_clusters->size(); ++i)
// 	// 	for (int j = 0; j < (*large_clusters)[i].indices.size(); ++j)
// 	// 		out->points[(*large_clusters)[i].indices[j]].intensity = +10.0;
// 	for (int i = 0; i < clusters->size(); ++i)
// 	{
// 		int label = rand() % 8;
// 		for (int j = 0; j < (*clusters)[i].indices.size(); ++j)
// 			out->points[(*clusters)[i].indices[j]].intensity = label;
// 	}


// }  


// void PointcloudDealCore::euclideanclustering_pt(double power_threshold,  pcl::PointCloud<pcl::PointXYZI>::Ptr in,
//                                    pcl::PointCloud<pcl::PointXYZI>::Ptr out,pcl::PointCloud<pcl::PointXYZI>::Ptr source)
// {
// 	std::vector<pcl::PointIndices> cluster_indices;
// 	pcl::search::KdTree<pcl::PointXYZI>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZI>);
// 	pcl::EuclideanClusterExtraction<pcl::PointXYZI> euclid;
// 	euclid.setInputCloud(in);
// 	euclid.setClusterTolerance(0.05);
// 	euclid.setMinClusterSize(MIN_CLUSTER_SIZE);
// 	euclid.setMaxClusterSize(MAX_CLUSTER_SIZE);
// 	euclid.setSearchMethod(tree);
// 	euclid.extract(cluster_indices);
// 	std::cout <<"cluster_indices size:"<<cluster_indices.size()<<std::endl;
// 	int point_count = 0;
// 	int j = 0;
// 	for (std::vector<pcl::PointIndices>::const_iterator  it = cluster_indices.begin(); it != cluster_indices.end(); ++it)
// 	{
// 		//通过下标，逐个填充
// 			point_count = 0;
// 			j++;
// 			for (std::vector<int>::const_iterator  pit = it->indices.begin(); pit != it->indices.end(); pit++)
// 			{	
// 				auto pp = source->points[*pit]; //将分类后的点云染色并发布
// 				pp.intensity = j*30;
// 				//pp.intensity = 30;
// 				out->points.push_back(pp); //*
// 				point_count++;
// 			}
// 			std::cout << "number"<< j <<"point_count "<<point_count<<std::endl;	

// 	}
// 	out->width = out->points.size();
// 	out->height = 1;
// 	out->is_dense = true;

// }
/*
欧几里得聚类
输入：去除地面点后的点云
输出：聚类后的点云组
*/
void PointcloudDealCore::euclideanclustering_pt(double ClusterTolerance,  pcl::PointCloud<pcl::PointXYZI>::Ptr in,
                                   std::vector<pcl::PointIndices>* cluster_indices)
{
	pcl::search::KdTree<pcl::PointXYZI>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZI>);
	pcl::EuclideanClusterExtraction<pcl::PointXYZI> euclid;
	euclid.setInputCloud(in);
	euclid.setClusterTolerance(ClusterTolerance);
	euclid.setMinClusterSize(MIN_CLUSTER_SIZE);
	euclid.setMaxClusterSize(MAX_CLUSTER_SIZE);
	euclid.setSearchMethod(tree);
	euclid.extract(*cluster_indices);
	// int point_count = 0;
	// int j = 0;
	// for (std::vector<pcl::PointIndices>::const_iterator  it = cluster_indices.begin(); it != cluster_indices.end(); ++it)
	// {
	// 	//通过下标，逐个填充
	// 		point_count = 0;
	// 		j++;
	// 		for (std::vector<int>::const_iterator  pit = it->indices.begin(); pit != it->indices.end(); pit++)
	// 		{	
	// 			auto pp = source->points[*pit]; //将分类后的点云染色并发布
	// 			pp.intensity = j*30;
	// 			//pp.intensity = 30;
	// 			out->points.push_back(pp); //*
	// 			point_count++;
	// 		}
	// 		std::cout << "number"<< j <<"point_count "<<point_count<<std::endl;	

	// }
	// out->width = out->points.size();
	// out->height = 1;
	// out->is_dense = true;

}


void PointcloudDealCore::point_cb(const sensor_msgs::PointCloud2ConstPtr &in_cloud_ptr)
{
	std::vector<pcl::PointIndices> cluster_indices;
    pcl::PointCloud<pcl::PointXYZI>::Ptr current_pc_ptr(new pcl::PointCloud<pcl::PointXYZI>);
    pcl::fromROSMsg(*in_cloud_ptr, *current_pc_ptr);
    pcl::PointCloud<pcl::PointXYZI>::Ptr pc_zip_to_2d_ptr(new pcl::PointCloud<pcl::PointXYZI>);
    pcl::PointCloud<pcl::PointXYZI>::Ptr pc_curvatured_ptr(new pcl::PointCloud<pcl::PointXYZI>);
    projection_3D_to_2D_pt(MIN_DISTANCE, current_pc_ptr, pc_zip_to_2d_ptr);
    euclideanclustering_pt(CLU_TOLERANCE, pc_zip_to_2d_ptr,&cluster_indices);
	std::cout <<"cluster_indices size:"<<cluster_indices->size()<<std::endl;
    //publish_cloud(pub_curvatured_ptr_, pc_curvatured_ptr, in_cloud_ptr->header);

}