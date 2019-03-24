//
// Created by ZelanXiao on 2019/3/13.
//

#ifndef PANORAMA_RANSAC_H
#define PANORAMA_RANSAC_H

#include <opencv2/opencv.hpp>
#include <random>
#include "match.h"
#include "functions.h"

using namespace cv;
using namespace std;

class Ransac{
public:;
    Ransac();
    void create(const Mat &gray_img1, const Mat &gray_img2);
    void doRANSAC(int numIterations, double inlierThreshold, Mat &homo, Mat &homInv);
    void displayInliersMatches(const Mat &img1, const Mat &img2);
private:
    //auxiliary functions
    void computeInlierCount(const Mat &homo, int &inliersCount, double inlierThreshold);
    void iterRANSAC(int numIterations, double inlierThreshold, Mat &homo);
    void computeRefinedHomography(const Mat &bestHomo, Mat &homo, Mat &homInv, double inlierThreshold);


    //data members
    vector<KeyPoint> keypoints_vec1;
    vector<KeyPoint> keypoints_vec2;
    vector<DMatch> good_matches;            //matches after ssd ratio test
    vector<DMatch> inliers_matches;         //inliers matches
};

#endif //PANORAMA_RANSAC_H
