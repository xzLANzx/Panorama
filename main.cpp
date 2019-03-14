#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <string>
#include "ransac.h"

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

void loadImage(Mat &src, string imgName) {
    src = imread(imgName, IMREAD_COLOR);
    if (!src.data) {
        cout << "Fail to load image!" << endl;
        exit(1);
    }
}

int main() {

    //load image
    Mat src;

    string imgName = "../project_images/MelakwaLake1.png";
    loadImage(src, imgName);


    //convert to gray
    Mat img_gray;
    cvtColor(src, img_gray, COLOR_BGR2GRAY);

    float a = sqrt(2) / 2;
    float b = -a;
    float c = 100;
    float d = a;
    float e = a;
    float f = 100;
    float g = 0;
    float h = 0;

    float homo_data[3][3] = {{a, b, c},
                             {d, e, f},
                             {g, h, 1}};
    Mat Homo = Mat(3, 3, CV_32F, &homo_data);

    Mat outImg = Mat(src.rows, src.cols, src.type());
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            float x1 = j;
            float y1 = i;
            float x2 = 0, y2 = 0;
            Point new_pt = project(x1, y1, Homo, x2, y2);

            //mark sure it's not going out of boundary
            if (new_pt.x >= 0 && new_pt.x < src.cols && new_pt.y >= 0 && new_pt.y < src.rows) {
                outImg.at<Vec3b>(new_pt.y, new_pt.x) = src.at<Vec3b>(y1, x1);
            }
        }
    }

    imshow("Result", outImg);


    vector<Point2f> src_pts_vec;
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            src_pts_vec.push_back(Point2f(j, i));
        }
    }

    vector<Point2f> dst_pts_vec;
    perspectiveTransform(src_pts_vec, dst_pts_vec, Homo);


    Mat dstImgMat = Mat(dst_pts_vec);
    Mat outImg2 = Mat::zeros(src.size(), src.type());

    for (size_t i = 0; i < src_pts_vec.size(); ++i) {

        float x1 = src_pts_vec[i].x;
        float y1 = src_pts_vec[i].y;

        float x2 = dst_pts_vec[i].x;
        float y2 = dst_pts_vec[i].y;

        //mark sure it's not going out of boundary
        if (x2 >= 0 && x2 < src.cols && y2 >= 0 && y2 < src.rows) {
            outImg2.at<Vec3b>(y2, x2) = src.at<Vec3b>(y1, x1);
        }
    }
    imshow("Result2", outImg2);


    waitKey(0);
    return 0;
}