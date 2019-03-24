//
// Created by ZelanXiao on 2019/3/19.
//

#ifndef PANORAMA_STITCH_H
#define PANORAMA_STITCH_H
#include <opencv2/opencv.hpp>
#include "ransac.h"

using namespace cv;
using namespace std;

class Stitch{
public:
    Stitch();
    void stitchImages(const Mat &color_img1, const Mat &color_img2, Mat &out_img);
    void computeHomo(const Mat& color_img1, const Mat& color_img2, Mat &homo, Mat &homInv);
    void computeStitchedImgInfo(const Mat &img1, const Mat &img2, const Mat &homInv);
    int getDirection();
private:
    //auxiliary functions
    void copyShiftBaseImg(const Mat &base_img, const Size &shfit, const Size &stitched_img_size, Mat &out_img);
    void stitchToBaseImg(Mat stitched_img, const Mat &img2, const Mat &homo, const Size &shift);

    //data members
    Size img1_shift;            //shift of image 1 in the new stitched image
    Size stitched_img_size;     //stitched image size
    int direction;              //stitched to the left or right of image 1
};
#endif //PANORAMA_STITCH_H
