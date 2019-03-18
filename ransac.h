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
        void doRANSAC(const vector<DMatch> &matches, int &numMatches, int numIterations, float inlierThreshold, const Mat &homo, const Mat &homInv, Mat &image1Display, Mat &image2Display);
    private:
        void computeInlierCount(const Mat &homo, const vector<DMatch> &matches, int &numMatches, float inlierThreshold);

        //auxiliary functions
        void project(const float &x1, const float &y1, const Mat &homo, float &x2, float &y2);
        void homographyTransform(const Mat &src_img, const Mat &homo, Mat &out_img);    //with self implemented project method
        void homographyTransform2(const Mat &src_img, const Mat &homo, Mat &out_img);   //with build-in perspective transform

        //data members
        vector<KeyPoint> keypoints_vec1;
        vector<KeyPoint> keypoints_vec2;
        vector<DMatch> good_matches;
    };
}








#endif //PANORAMA_RANSAC_H
