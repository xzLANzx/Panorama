//
// Created by ZelanXiao on 2019/3/17.
//

#include "harrisCornerDetection.h"

HarrisCornerDetection::HarrisCornerDetection(){
}

void HarrisCornerDetection::computeHarrisResponse(const Mat &gray_img, int blockSize, int apertureSize, double k){
    harrisResponseMat = Mat::zeros(gray_img.size(), CV_32FC1);
    cornerHarris(gray_img, harrisResponseMat, blockSize, apertureSize, k);

    Mat dst_norm, dst_norm_scaled;
    normalize(harrisResponseMat, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
    convertScaleAbs(dst_norm, dst_norm_scaled);
    dst_norm.copyTo(harrisResponseMat);

}

void HarrisCornerDetection::detectKeyPoints(vector<KeyPoint> &keypoints_vec, float thresh){
    for (size_t i = 0; i < harrisResponseMat.rows; ++i) {
        for (size_t j = 0; j < harrisResponseMat.cols; ++j) {
            if (harrisResponseMat.at<float>(i, j) > thresh) {
                KeyPoint kpt(j, i, 3);
                kpt.response = harrisResponseMat.at<float>(i, j);
                keypoints_vec.push_back(kpt);
            }
        }
    }
    cout << "Keypoints Count: " << keypoints_vec.size() << endl;
}