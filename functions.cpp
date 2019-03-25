//
// Created by ZelanXiao on 2019/3/19.
//

#include "functions.h"

void project(const double &x1, const double &y1, const Mat &homo, double &x2, double &y2) {

    Mat src_pt = Mat::zeros(3, 1, homo.type());
    src_pt.at<double>(0, 0) = x1;
    src_pt.at<double>(1, 0) = y1;
    src_pt.at<double>(2, 0) = 1;

    Mat dst_pt = homo * src_pt;
    double w = dst_pt.at<double>(2, 0);

    x2 = dst_pt.at<double>(0, 0) /= w;
    y2 = dst_pt.at<double>(1, 0) /= w;
}