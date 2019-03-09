//
// Created by ZelanXiao on 2019/3/8.
//

#ifndef PANORAMA_HARRISCORNER_H
#define PANORAMA_HARRISCORNER_H

#include<opencv2/opencv.hpp>
#include<iostream>
#include<string>

using namespace cv;
using namespace std;

void computeCornerStrengthMat(const Mat &img, Mat &corner_strength_mat);

void getKeypointsFromCornerStrengthMat(const Mat &corner_strength_mat, vector<KeyPoint> &keypoints_vec);

void displayKeypoints(const vector<KeyPoint> &keypoints_vec, const Mat &img);

#endif //PANORAMA_HARRISCORNER_H
