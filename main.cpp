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
    //TODO: c++ class big three
    //load image
    string img_name1 = "../project_images/MelakwaLake2.png";
    string img_name2 = "../project_images/MelakwaLake3.png";

    Mat gray_img1 = imread(img_name1, IMREAD_GRAYSCALE);
    Mat gray_img2 = imread(img_name2, IMREAD_GRAYSCALE);

    Mat color_img1 = imread(img_name1, IMREAD_COLOR);
    Mat color_img2 = imread(img_name2, IMREAD_COLOR);

    if (!gray_img1.data || !gray_img2.data) {
        cout << "Fail to load image!" << endl;
        exit(1);
    }


    int numIterations = 30;
    double inlierThreshold = 2;
    Mat homo, homInv;

    self::RANSAC *rsc = new self::RANSAC();
    rsc->create(gray_img1, gray_img2);
    rsc->doRANSAC(numIterations, inlierThreshold, homo, homInv, color_img1, color_img2);

//    rsc->iterRANSAC(numIterations, inlierThreshold, homo);
//    rsc->computeRefinedHomography(homo, homo, homInv, inlierThreshold);
//    rsc->displayInliersMatches(color_img1, color_img2);
//    rsc->doRANSAC2(homo);

//    Mat outImage;
//    cout << homo << endl;
//    rsc->homographyTransform(color_img1, homo, outImage);


    waitKey(0);
    return 0;
}