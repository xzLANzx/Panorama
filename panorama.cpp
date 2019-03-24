//
// Created by ZelanXiao on 2019/3/21.
//

#include "panorama.h"
#include "stitch.h"

void Panorama::create(int imgs_count, string img_name) {

    img_path = "../project_images/";
    this->img_name = img_name;
    total_imgs_count = imgs_count;
    for (size_t i = 0; i < imgs_count; ++i) {
        unstitched_imgs_vec.push_back(i + 1);
    }
}

void Panorama::stitch2(int index) {
//    if (stitched_images_count == 0) {
//
//        base_image = imread(image_paths_vec[index], IMREAD_COLOR);
//        stitched_images_count = 1;
//    } else if (stitched_images_count == 1) {
//
//        //stitch the one with base image
//        Mat color_img2 = imread(image_paths_vec[index], IMREAD_COLOR);
//        Stitch *stitcher = new Stitch();
//        Mat out_img;
//        stitcher->stitchImages(base_image, color_img2, out_img);
//        out_img.copyTo(base_image);
//        stitched_images_count++;
//        direction = stitcher->getDirection();
//        cout << "Stitched to: " << direction << endl;
//    } else if (stitched_images_count < images_count) {
//
//        //get the image 2 to stitch
//        Mat color_img2 = imread(image_paths_vec[index], IMREAD_COLOR);
//
//        int prev_direction = direction;
//
//        //compute the new stitching direction
//        Stitch *stitcher = new Stitch();
//        Mat homo, homInv;
//        stitcher->computeHomo(base_image, color_img2, homo, homInv);
//        stitcher->computeStitchedImgInfo(base_image, color_img2, homInv);
//        int next_direction = stitcher->getDirection();
//
//    }
//
//    imshow("Base Image", base_image);
}

void Panorama::countMatches() {

    string path = img_path + img_name;
    int unstitched_imgs_count = unstitched_imgs_vec.size();
    if (unstitched_imgs_count == total_imgs_count) {

        matches_count.clear();
        vector<int> row(unstitched_imgs_count);
        matches_count = vector<vector<int>>(unstitched_imgs_count);
        for (size_t i = 0; i < unstitched_imgs_count; ++i)
            matches_count[i] = row;

        //compute match count and pick two
        for (size_t i = 0; i < unstitched_imgs_count; ++i) {
            for (size_t j = i + 1; j < unstitched_imgs_count; ++j) {

                string img1_path = path + to_string(unstitched_imgs_vec[i]) + ".png";
                string img2_path = path + to_string(unstitched_imgs_vec[j]) + ".png";
                Mat color_img1 = imread(img1_path, IMREAD_COLOR);
                Mat color_img2 = imread(img2_path, IMREAD_COLOR);

                Match *matcher = new Match();
                matcher->findMatches(color_img1, color_img2);
                int num_matches = matcher->getMatchesCount();
                matches_count[i][j] = num_matches;
                matches_count[j][i] = num_matches;
            }
        }
    } else {
        //compute match count between baseImage and other images
    }
}

void Panorama::pickTwoImgs(int &max_i, int &max_j) {

    int max_matches = 0;
    max_i = 0;
    max_j = max_i + 1;

    for (size_t i = 0; i < matches_count.size(); ++i) {
        for (size_t j = i + 1; j < matches_count[i].size(); ++j) {
            if (matches_count[i][j] > max_matches) {
                max_matches = matches_count[i][j];
                max_i = i;
                max_j = j;
            }
        }
    }
}

void Panorama::stitch() {
    int i = 0, j = i + 1;
    pickTwoImgs(i, j);

    string img1_path = img_path + img_name + to_string(unstitched_imgs_vec[i]) + ".png";
    string img2_path = img_path + img_name + to_string(unstitched_imgs_vec[j]) + ".png";
    Mat color_img1 = imread(img1_path, IMREAD_COLOR);
    Mat color_img2 = imread(img2_path, IMREAD_COLOR);

    //stitch
    Stitch *stitcher = new Stitch();
    Mat out_img;
    stitcher->stitchImages(color_img1, color_img2, out_img);

    //save the out_img to file with a new name
    imshow("Test", out_img);
}


void Panorama::printMatchesCount() {
    for (size_t i = 0; i < matches_count.size(); ++i) {
        for (size_t j = 0; j < matches_count[i].size(); ++j) {
            cout << matches_count[i][j] << ", ";
        }
        cout << endl;
    }
}


















