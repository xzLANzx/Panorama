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
}

void Panorama::stitchToBase(int i) {
    if (stitched_count == 0) {
        string img2_path = img_path + img_name + to_string(i) + ".png";
        Mat color_img2 = imread(img2_path, IMREAD_COLOR);
        color_img2.copyTo(base_img);
        stitched_count++;
        return;
    } else {
        string img2_path = img_path + img_name + to_string(i) + ".png";
        Mat color_img2 = imread(img2_path, IMREAD_COLOR);

        //stitch
        Stitch *stitcher = new Stitch();
        Mat out_img;
        stitcher->stitchImages(base_img, color_img2, out_img);
        out_img.copyTo(base_img);
        stitched_count++;
    }

    if (stitched_count == imgs_count) {
        //save the final_img to file with a new name
        imwrite("../project_images/" + img_name + "_Result.png", base_img);
        imshow("Stitching Result", base_img);
    }

}

void Panorama::stitchAll(int imgs_count, string img_name) {

    create(imgs_count, img_name);

    if(imgs_count == 4 && img_name == "MelakwaLake"){
        stitchToBase(3);
        stitchToBase(2);
        stitchToBase(1);
        stitchToBase(4);
        return;
    }

    if(imgs_count == 6 && img_name == "Rainier"){
        stitchToBase(1);
        stitchToBase(5);
        stitchToBase(3);
        stitchToBase(4);
        stitchToBase(6);
        stitchToBase(2);
        return;
    }

    for (size_t img_id = 1; img_id <= imgs_count; img_id++) {
        stitchToBase(img_id);
    }
}