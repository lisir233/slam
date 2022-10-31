//
// Created by adam on 18-9-21.
//

#include "pointcloud_deal_core.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "pointcloud_deal");

    ros::NodeHandle nh;

    PointcloudDealCore core(nh);
    // core.Spin();
    return 0;
}