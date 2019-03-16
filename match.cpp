//
// Created by ZelanXiao on 2019/3/15.
//

#include "match.h"

void findMatches() {
    Mat img1 = imread("../project_images/MelakwaLake1.png", IMREAD_GRAYSCALE);
    Mat img2 = imread("../project_images/MelakwaLake2.png", IMREAD_GRAYSCALE);

    if (!img1.data || !img2.data) {
        cout << "Fail to load image!" << endl;
        exit(1);
    }

    Ptr<SIFT> sift = SIFT::create(0, 4, 0.04, 10, 1.6);
    vector<KeyPoint> kpts_vec1, kpts_vec2;

    //detect key points
    sift->detect(img1, kpts_vec1);
    sift->detect(img2, kpts_vec2);

    //compute sift feature descriptors
    Mat descriptors1, descriptors2;
    sift->compute(img1, kpts_vec1, descriptors1);
    sift->compute(img2, kpts_vec2, descriptors2);

    //match descriptor vectors using BFMatcher
    BFMatcher matcher;
    vector<vector<DMatch>> matches;
    matcher.knnMatch(descriptors1, descriptors2, matches, 2);
//    matcher.match( descriptors1, descriptors2, matches );

    vector<DMatch> good_matches;
    for (size_t i = 0; i < matches.size(); ++i) {
        DMatch match1 = matches[i][0];
        DMatch match2 = matches[i][1];
        cout << i << ": " << match1.distance << "\t" << match2.distance << endl;
        if (match1.distance < (0.75 * match2.distance)) {
            good_matches.push_back(match1);
        }
    }

    //draw matches
    Mat matches_img;
    drawMatches(img1, kpts_vec1, img2, kpts_vec2, good_matches, matches_img);
    imshow("Matches", matches_img);
}