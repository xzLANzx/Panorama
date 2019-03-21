#include <string>
#include "ransac.h"
#include "stitch.h"
#include "functions.h"

using namespace cv;
using namespace std;


int main() {
    //TODO: c++ class big three
    //load image
    string img_name1 = "../project_images/Hanging1.png";
    string img_name2 = "../project_images/Hanging2.png";

    Mat gray_img1 = imread(img_name1, IMREAD_GRAYSCALE);
    Mat gray_img2 = imread(img_name2, IMREAD_GRAYSCALE);

    Mat color_img1 = imread(img_name1, IMREAD_COLOR);
    Mat color_img2 = imread(img_name2, IMREAD_COLOR);

    if (!gray_img1.data || !gray_img2.data) {
        cout << "Fail to load image!" << endl;
        exit(1);
    }


    //RANSAC
    int numIterations = 30;
    double inlierThreshold = 2;
    Mat homo, homInv;
    self::RANSAC *rsc = new self::RANSAC();
    rsc->create(gray_img1, gray_img2);
    rsc->doRANSAC(numIterations, inlierThreshold, homo, homInv, color_img1, color_img2);


    //Stitching
    Stitch *stitcher = new Stitch();
    Size img1_shift = Size(0, 0);
    Size stitched_img_size = stitcher->computeStitchedImgSize(color_img1, color_img2, homInv, img1_shift);
    Mat out_img;
    stitcher->copyShiftBaseImg(color_img1, img1_shift, stitched_img_size, out_img);
    stitcher->stitchToBaseImg(out_img, color_img2, color_img1, homo, img1_shift);
    imshow("Final", out_img);

    waitKey(0);
    return 0;
}