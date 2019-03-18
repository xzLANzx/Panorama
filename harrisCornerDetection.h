//
// Created by ZelanXiao on 2019/3/17.
//

#ifndef PANORAMA_HARRISCORNERDETECTION_H
#define PANORAMA_HARRISCORNERDETECTION_H
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class HarrisCornerDetection{
public:
    HarrisCornerDetection();
    void detectKeyPoints(vector<KeyPoint> &keypoints_vec, float thresh);
    void computeHarrisResponse(const Mat &gray_img, int blockSize, int apertureSize, double k);
private:
    //data member
    Mat harrisResponseMat;
};
#endif //PANORAMA_HARRISCORNERDETECTION_H
