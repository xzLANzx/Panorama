//
// Created by ZelanXiao on 2019/3/19.
//

#include "stitch.h"
#include "functions.h"

Stitch::Stitch() {
}

Size Stitch::computeStitchedImgSize(const Mat &homInv, Mat &img2, double &x_shift, double &y_shift) {
    vector<Point2d> img2_corners(4);
    img2_corners[0] = Point2d(0, 0);
    img2_corners[1] = Point2d((double) img2.cols, 0);
    img2_corners[2] = Point2d((double) img2.cols, (double) img2.rows);
    img2_corners[3] = Point2d(0, (double) img2.rows);

    project(img2_corners[0].x, img2_corners[0].y, homInv, img2_corners[0].x, img2_corners[0].y);
    cout << img2_corners[0].x << ", " << img2_corners[0].y << endl;

    project(img2_corners[1].x, img2_corners[1].y, homInv, img2_corners[1].x, img2_corners[1].y);
    cout << img2_corners[1].x << ", " << img2_corners[1].y << endl;

    project(img2_corners[2].x, img2_corners[2].y, homInv, img2_corners[2].x, img2_corners[2].y);
    cout << img2_corners[2].x << ", " << img2_corners[2].y << endl;

    project(img2_corners[3].x, img2_corners[3].y, homInv, img2_corners[3].x, img2_corners[3].y);
    cout << img2_corners[3].x << ", " << img2_corners[3].y << endl;

    //width = max(x) - min(x)
    double min_x = img2_corners[0].x;
    double max_x = img2_corners[0].x;
    double min_y = img2_corners[0].y;
    double max_y = img2_corners[0].y;

    for (size_t i = 0; i < img2_corners.size(); ++i) {
        if (img2_corners[i].x < min_x) min_x = img2_corners[i].x;
        if (img2_corners[i].x > max_x) max_x = img2_corners[i].x;

        if (img2_corners[i].y < min_y) min_y = img2_corners[i].y;
        if (img2_corners[i].y > max_y) max_y = img2_corners[i].y;
    }

    double width = max_x - min_x;
    double height = max_y - min_y;
    Size stitchedImgSize = Size(width, height);

    x_shift = min_x;
    y_shift = min_y;

    return stitchedImgSize;
}

Size Stitch::computeStitchedImgSize(const Mat &img1, const Mat &img2, const Mat &homInv, Size &img1_shift) {

    //get the 4 corners of image 1 and image 2
    vector<Point2d> img1_corners(4);
    img1_corners[0] = Point2d(0, 0);
    img1_corners[1] = Point2d((double) img1.cols, 0);
    img1_corners[2] = Point2d((double) img1.cols, (double) img1.rows);
    img1_corners[3] = Point2d(0, (double) img1.rows);

    vector<Point2d> img2_corners(4);
    img2_corners[0] = Point2d(0, 0);
    img2_corners[1] = Point2d((double) img2.cols, 0);
    img2_corners[2] = Point2d((double) img2.cols, (double) img2.rows);
    img2_corners[3] = Point2d(0, (double) img2.rows);

    //project image 2 corners to image 1
    for (size_t i = 0; i < img2_corners.size(); ++i) {
        project(img2_corners[i].x, img2_corners[i].y, homInv, img2_corners[i].x, img2_corners[i].y);
    }

    //find min_x, min_y, max_x, max_y
    double min_x = img1_corners[0].x;
    double max_x = img1_corners[0].x;
    double min_y = img1_corners[0].y;
    double max_y = img1_corners[0].y;

    for (size_t i = 0; i < img1_corners.size(); ++i) {
        if (img1_corners[i].x < min_x) min_x = img1_corners[i].x;
        if (img1_corners[i].x > max_x) max_x = img1_corners[i].x;
        if (img1_corners[i].y < min_y) min_y = img1_corners[i].y;
        if (img1_corners[i].y > max_y) max_y = img1_corners[i].y;
    }

    for (size_t i = 0; i < img2_corners.size(); ++i) {
        if (img2_corners[i].x < min_x) min_x = img2_corners[i].x;
        if (img2_corners[i].x > max_x) max_x = img2_corners[i].x;
        if (img2_corners[i].y < min_y) min_y = img2_corners[i].y;
        if (img2_corners[i].y > max_y) max_y = img2_corners[i].y;
    }
    cout << "min x: " << min_x << ", max x: " << max_x << endl;
    cout << "min y: " << min_y << ", max y: " << max_y << endl;

    img1_shift = Size((0 - min_x), (0 - min_y));


    return Size((max_x - min_x), (max_y - min_y));
}

void Stitch::copyShiftBaseImg(const Mat &base_img, const Size &shift, const Size &stitched_img_size, Mat &out_img) {
    out_img = Mat::zeros(stitched_img_size, base_img.type());
    cout << base_img.size() << endl;

    double shift_x = shift.width;
    double shift_y = shift.height;

    cout << "shift x: " << shift_x << endl;
    cout << "shift y: " << shift_y << endl;

    for (size_t i = 0; i < base_img.rows; ++i) {
        for (size_t j = 0; j < base_img.cols; ++j) {
            out_img.at<Vec3b>(i + shift_y, j + shift_x) = base_img.at<Vec3b>(i, j);
        }
    }

    //imshow("Result", out_img);
}

void Stitch::stitchToBaseImg(Mat stitched_img, const Mat &img2, const Mat &img1, const Mat &homo, const Size &shift) {
    double x_shift = shift.width;
    double y_shift = shift.height;
    for (size_t i = 0; i < stitched_img.rows; i++) {
        for (size_t j = 0; j < stitched_img.cols; j++) {
            double x1 = j - x_shift;
            double y1 = i - y_shift;
            double x2 = 0, y2 = 0;
            project(x1, y1, homo, x2, y2);

            //add or blend img2 to stitched image
            if (x2 >= 0 && x2 < (img2.cols - 1) && y2 >= 0 && y2 < (img2.rows - 1)) {
                Point2d center = Point2d(x2, y2);
                Mat pixel = Mat::zeros(1, 1, img2.type());
                getRectSubPix(img2, pixel.size(), center, pixel);

                Vec3b basePx = stitched_img.at<Vec3b>(i, j);
                if (basePx[0] == 0 && basePx[1] == 0 && basePx[2] == 0) {
                    //base pixel is black, add image 2 color
                    stitched_img.at<Vec3b>(i, j) = pixel.at<Vec3b>(0, 0);
                } else {
                    //blend
                    stitched_img.at<Vec3b>(i, j) = (stitched_img.at<Vec3b>(i, j) + pixel.at<Vec3b>(0, 0)) / 2;
                }
            }
        }
    }
}