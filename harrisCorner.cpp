//
// Created by ZelanXiao on 2019/3/8.
//
#include "harrisCorner.h"

int thresh = 200;
const char* corners_window = "Corners detected";

void computeCornerStrengthMat(const Mat &img, Mat &corner_strength_mat){

    //convert to gray
    Mat img_gray;
    cvtColor( img, img_gray, COLOR_BGR2GRAY );

    //get harris corners
    //https://docs.opencv.org/4.0.1/d4/d7d/tutorial_harris_detector.html
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;
    Mat result = Mat::zeros( img.size(), CV_32FC1 );
    cornerHarris( img_gray, result, blockSize, apertureSize, k );

    //normalize result to 0~255
    normalize( result, corner_strength_mat, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
}

void getKeypointsFromCornerStrengthMat(const Mat &corner_strength_mat, vector<KeyPoint> &keypoints_vec){

    for( int i = 0; i < corner_strength_mat.rows ; i++ )
    {
        for( int j = 0; j < corner_strength_mat.cols; j++ )
        {
            if( (int) corner_strength_mat.at<float>(i,j) > thresh )
            {
                float corner_strength = corner_strength_mat.at<float>(i,j);
                KeyPoint kpt(j, i, 3);
                kpt.response = corner_strength;
                keypoints_vec.push_back(kpt);
            }
        }
    }
}

void displayKeypoints(const vector<KeyPoint> &keypoints_vec, const Mat &img){

    Mat out_img;
    drawKeypoints(img, keypoints_vec, out_img);

    namedWindow( corners_window );
    imshow( corners_window, out_img );
}
