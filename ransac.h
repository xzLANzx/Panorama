//
// Created by ZelanXiao on 2019/3/13.
//

#ifndef PANORAMA_RANSAC_H
#define PANORAMA_RANSAC_H

#include <opencv2/opencv.hpp>
#include <random>
#include "match.h"
using namespace cv;
using namespace std;

namespace self{
    class RANSAC{
    public:;
        RANSAC();
        void create(const Mat &gray_img1, const Mat &gray_img2);
        void doRANSAC(int numIterations, double inlierThreshold, Mat &homo, Mat &homInv, const Mat &displayImage1, const Mat &displayImage2);
        void homographyTransform(const Mat &src_img, const Mat &homo, Mat &out_img);    //with self implemented project method
        void homographyTransform2(const Mat &src_img, const Mat &homo, Mat &out_img);   //with build-in perspective transform
    private:
        //auxiliary functions
        void project(const double &x1, const double &y1, const Mat &homo, double &x2, double &y2);
        void computeInlierCount(const Mat &homo, int &inliersCount, double inlierThreshold);
        void iterRANSAC(int numIterations, double inlierThreshold, Mat &homo);
        void computeRefinedHomography(const Mat &bestHomo, Mat &homo, Mat &homInv, double inlierThreshold);
        void displayInliersMatches(const Mat &img1, const Mat &img2);


        //data members
        vector<KeyPoint> keypoints_vec1;
        vector<KeyPoint> keypoints_vec2;
        vector<DMatch> good_matches;            //match after ssd
        vector<DMatch> inliers_matches;         //inliers match
    };
}








#endif //PANORAMA_RANSAC_H
