//
// Created by ZelanXiao on 2019/3/19.
//

#ifndef PANORAMA_STITCH_H
#define PANORAMA_STITCH_H
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class Stitch{
public:
    Stitch();
    Size computeStitchedImgSize(const Mat &homInv, Mat &img2, double &x_shift, double &y_shift);
    Size computeStitchedImgSize(const Mat &img1, const Mat &img2, const Mat &homInv, Size &img1_shfit);
    void copyShiftBaseImg(const Mat &base_img, const Size &shfit, const Size &stitched_img_size, Mat &out_img);
    void stitchToBaseImg(Mat stitched_img, const Mat &img2, const Mat &img1, const Mat &homo, const Size &shift);
private:
};
#endif //PANORAMA_STITCH_H
