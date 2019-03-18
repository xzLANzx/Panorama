//
// Created by ZelanXiao on 2019/3/15.
//

#ifndef PANORAMA_MATCH_H
#define PANORAMA_MATCH_H

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include "harrisCornerDetection.h"

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

class Match{
public:
    Match();
    void findMatches(const Mat &gray_img1, const Mat &gray_img2);

    vector<KeyPoint> getKeypointsVec(int i);
    Mat getFeatureDescriptors(int i);
    vector<DMatch> getMatches();

private:
    //auxiliary functions

    //get all the keypoints and their corresponding feature descriptors of an image
    void computeNormSIFTDescriptors(const Mat &gray_img, vector<KeyPoint> &keypoints_vec, Mat &descriptors_vec);

    //normalize all the feature descriptors
    void normalizeFeatureDescriptors(const Mat &descriptors, Mat &norm_descriptors);

    //SSD ratio test
    void SSDRatioTest(const vector<vector<DMatch>> &matches, float ratio, float thresh);

    //data members
    vector<KeyPoint> keypoints_vec1;
    vector<KeyPoint> keypoints_vec2;
    Mat descriptors_vec1;
    Mat descriptors_vec2;
    vector<DMatch> good_matches;
};



#endif //PANORAMA_MATCH_H
