//#include <opencv2/opencv.hpp>
//#include <opencv2/xfeatures2d.hpp>
#include <string>
#include "ransac.h"
#include "match.h"

using namespace cv;
//using namespace cv::xfeatures2d;
using namespace std;

void loadImage(Mat &src, string imgName) {
    src = imread(imgName, IMREAD_COLOR);
    if (!src.data) {
        cout << "Fail to load image!" << endl;
        exit(1);
    }
}

void getHomographyMat(Mat &Homo) {

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

    Mat h_mat = Mat(3, 3, CV_32F, &homo_data);
    h_mat.copyTo(Homo);
}

int main() {

//    //load image
//    Mat src;
//    string imgName = "../project_images/MelakwaLake1.png";
//    loadImage(src, imgName);
//
//    //get homography matrix
//    Mat homo;
//    getHomographyMat(homo);
//
//    //get image after homography transform
//    Mat outImage;
//    homographyTransform(src, homo, outImage);
//    //homographyTransform2(src, homo, outImage);

    findMatches();





    waitKey(0);
    return 0;
}