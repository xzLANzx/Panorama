//
// Created by ZelanXiao on 2019/3/21.
//

#include "panorama.h"
#include "stitch.h"

void Panorama::create(int imgs_count, string img_name) {

    img_path = "../project_images/";
    this->img_name = img_name;
    this->imgs_count = imgs_count;
    stitched_count = 0;
    max_id = imgs_count;
}

void Panorama::stitchToBase(int i) {
    if(stitched_count == 0){
        string img2_path = img_path + img_name + to_string(i) + ".png";
        Mat color_img2 = imread(img2_path, IMREAD_COLOR);
        color_img2.copyTo(base_img);
        stitched_count++;
        return;
    }else{
        string img2_path = img_path + img_name + to_string(i) + ".png";
        Mat color_img2 = imread(img2_path, IMREAD_COLOR);

        //stitch
        Stitch *stitcher = new Stitch();
        Mat out_img;
        stitcher->stitchImages(base_img, color_img2, out_img);
        out_img.copyTo(base_img);
        stitched_count++;
    }

    if (stitched_count == imgs_count){
        //save the final_img to file with a new name
        string out_img_name = generateNewImgName();
        //imwrite((img_path + out_img_name), base_img);
        imwrite("../project_images/final.png", base_img);
        imshow("Final", base_img);
    }

}

string Panorama::generateNewImgName() {
    string new_name = img_name + to_string(max_id + 1) + ".png";
    max_id++;
    return new_name;
}