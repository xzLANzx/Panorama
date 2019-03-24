//
// Created by ZelanXiao on 2019/3/21.
//

#include "panorama.h"
#include "stitch.h"

void Panorama::create(int imgs_count, string img_name) {

    img_path = "../project_images/";
    this->img_name = img_name;
    max_id = imgs_count;
    for (size_t i = 0; i < imgs_count; ++i) {
        unstitched_imgs_vec.push_back(i + 1);
    }
    countMatches();
}

void Panorama::countMatches() {

    int unstitched_imgs_count = unstitched_imgs_vec.size();
    matches_count.clear();
    vector<int> row(unstitched_imgs_count);
    matches_count = vector<vector<int>>(unstitched_imgs_count);
    for (size_t i = 0; i < unstitched_imgs_count; ++i)
        matches_count[i] = row;

    //compute matches count
    string path = img_path + img_name;
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

    //pick two images from matches count
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
    string out_img_name = generateNewImgName();
    imwrite((img_path + out_img_name), out_img);
    update(i, j);
    imshow("Test", out_img);
}

void Panorama::printMatchesCount() {
    for (size_t i = 0; i < matches_count.size(); ++i) {
        for (size_t j = 0; j < matches_count[i].size(); ++j) {
            cout << matches_count[i][j] << ", ";
        }
        cout << endl;
    }
    cout << endl;
}

string Panorama::generateNewImgName() {
    string new_name = img_name + to_string(max_id + 1) + ".png";
    max_id++;
    return new_name;
}

//update the info of unstitched images
void Panorama::update(int i, int j) {
    //remove i and j from the unstitched_imgs_vec
    vector<int> new_unstitched_imgs_vec;
    for (size_t k = 0; k < unstitched_imgs_vec.size(); ++k) {
        if (k != i && k != j) {
            new_unstitched_imgs_vec.push_back(unstitched_imgs_vec[k]);
        }
    }
    new_unstitched_imgs_vec.push_back(max_id);
    unstitched_imgs_vec = new_unstitched_imgs_vec;
}















