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
    void countMatches();
    void pickTwoImgs(int &max_i, int &max_j);   //return the index of the two pictures to stitch
    void stitch();
    void printMatchesCount();

    void stitch2(int index);
private:
    //auxiliary functions


    //data members
    Mat base_image;
    string img_path;
    string img_name;
    int total_imgs_count;               //number of all the images to be stitched
    vector<int> unstitched_imgs_vec;    //store the index of unstitched images
    vector<vector<int>> matches_count;  //store num of matches between images
};

#endif //PANORAMA_PANORAMA_H
