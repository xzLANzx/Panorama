//
// Created by ZelanXiao on 2019/3/13.
//

#include "ransac.h"

void project(const float &x1, const float &y1, Mat Homo, float &x2, float &y2) {
    Mat src_pt = Mat::zeros(3, 1, CV_32F);
    src_pt.at<float>(0, 0) = x1;
    src_pt.at<float>(1, 0) = y1;
    src_pt.at<float>(2, 0) = 1;

    Mat dst_pt = Homo * src_pt;
    float w = dst_pt.at<float>(2, 0);

    x2 = dst_pt.at<float>(0, 0) /= w;
    y2 = dst_pt.at<float>(0, 1) /= w;
}

//get image after homography transform
void homographyTransform(const Mat &src_img, const Mat &homo, Mat &out_img){

    out_img = Mat(src_img.rows, src_img.cols, src_img.type());

    for (int i = 0; i < src_img.rows; ++i) {
        for (int j = 0; j < src_img.cols; ++j) {
            float x1 = j, y1 = i;
            float x2 = 0, y2 = 0;
            project(x1, y1, homo, x2, y2);

            //mark sure it's not going out of boundary
            if (x2 >= 0 && x2 < src_img.cols && y2 >= 0 && y2 < src_img.rows) {
                out_img.at<Vec3b>(y2, x2) = src_img.at<Vec3b>(y1, x1);
            }
        }
    }

    imshow("Result", out_img);
}

void homographyTransform2(const Mat &src_img, const Mat &homo, Mat &out_img){

    vector<Point2f> src_pts_vec;
    for (int i = 0; i < src_img.rows; ++i) {
        for (int j = 0; j < src_img.cols; ++j) {
            src_pts_vec.push_back(Point2f(j, i));
        }
    }

    vector<Point2f> dst_pts_vec;
    perspectiveTransform(src_pts_vec, dst_pts_vec, homo);

    out_img = Mat::zeros(src_img.size(), src_img.type());
    for (size_t i = 0; i < src_pts_vec.size(); ++i) {

        float x1 = src_pts_vec[i].x;
        float y1 = src_pts_vec[i].y;

        float x2 = dst_pts_vec[i].x;
        float y2 = dst_pts_vec[i].y;

        //mark sure it's not going out of boundary
        if (x2 >= 0 && x2 < src_img.cols && y2 >= 0 && y2 < src_img.rows) {
            out_img.at<Vec3b>(y2, x2) = src_img.at<Vec3b>(y1, x1);
        }
    }
    imshow("Result2", out_img);
}

//computes the number of inlying points given a homography H
void computeInlierCount(const Mat &homo, const vector<DMatch> &matches, int &numMatches, int inlierThreshold){
    for(size_t i = 0; i < matches.size(); ++i){
        DMatch pts_pair = matches[i];
        if(pts_pair.distance < inlierThreshold){
            numMatches ++;
        }
    }
}