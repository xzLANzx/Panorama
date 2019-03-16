//
// Created by ZelanXiao on 2019/3/13.
//

#ifndef PANORAMA_RANSAC_H
#define PANORAMA_RANSAC_H

#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void project(const float &x1, const float &y1, Mat Homo, float &x2, float &y2);

void homographyTransform(const Mat &src_img, const Mat &homo, Mat &out_img);

void homographyTransform2(const Mat &src_img, const Mat &homo, Mat &out_img);

void computeInlierCount(const Mat &homo, DMatch &dMatch, int numMatches, int inlierThreshold);
#endif //PANORAMA_RANSAC_H
