//
// Created by ZelanXiao on 2019/3/21.
//

#ifndef PANORAMA_PANORAMA_H
#define PANORAMA_PANORAMA_H
#include <opencv2/opencv.hpp>
#include <string>
#include "match.h"
using namespace cv;
using namespace std;

class Panorama{
public:
    void create(int imgs_count, string img_name);
    void stitchToBase(int i);

private:
    //auxiliary functions
    string generateNewImgName();

    //data members
    string img_path;
    string img_name;
    int imgs_count;
    int stitched_count;
    Mat base_img;

    int max_id;                         //last id of saved image
};

#endif //PANORAMA_PANORAMA_H
