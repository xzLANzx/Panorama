//
// Created by ZelanXiao on 2019/3/19.
//

#ifndef PANORAMA_FUNCTIONS_H
#define PANORAMA_FUNCTIONS_H
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void project(const double &x1, const double &y1, const Mat &homo, double &x2, double &y2);
void homographyTransform(const Mat &src_img, Size dst_img_size, const Mat &homo, double x_shift, double y_shift);
void homographyTransform2(const Mat &src_img, const Mat &homo, Mat &out_img);    //with self implemented project method
void homographyTransform3(const Mat &src_img, const Mat &homo, Mat &out_img);   //with build-in perspective transform

#endif //PANORAMA_FUNCTIONS_H
