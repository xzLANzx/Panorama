//
// Created by ZelanXiao on 2019/3/19.
//

#ifndef PANORAMA_FUNCTIONS_H
#define PANORAMA_FUNCTIONS_H
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void project(const double &x1, const double &y1, const Mat &homo, double &x2, double &y2);

#endif //PANORAMA_FUNCTIONS_H
