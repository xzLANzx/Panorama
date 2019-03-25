//
// Created by ZelanXiao on 2019/3/13.
//

#include "ransac.h"
Ransac::Ransac() {
}

void Ransac::create(const Mat &gray_img1, const Mat &gray_img2) {

    Match *matcher = new Match();
    matcher->findMatches(gray_img1, gray_img2);
    keypoints_vec1 = matcher->getKeypointsVec(1);
    keypoints_vec2 = matcher->getKeypointsVec(2);
    good_matches = matcher->getMatches();
}

//computes the number of inlying points given a homography H
void Ransac::computeInlierCount(const Mat &homo, int &inliersCount, double inlierThreshold) {
    for (size_t i = 0; i < good_matches.size(); ++i) {

        DMatch pts_pair = good_matches[i];
        int idx1 = pts_pair.queryIdx;
        int idx2 = pts_pair.trainIdx;
        KeyPoint kpt1 = keypoints_vec1[idx1];
        KeyPoint kpt2 = keypoints_vec2[idx2];

        double x1 = kpt1.pt.x;
        double y1 = kpt1.pt.y;
        double x2 = 0, y2 = 0;

        //compute perspective transform
        project(x1, y1, homo, x2, y2);

        //compute the distance
        double x22 = kpt2.pt.x;
        double y22 = kpt2.pt.y;
        double distance = sqrt(pow((x2 - x22), 2) + pow((y2 - y22), 2));

        if (distance < inlierThreshold)
            inliersCount++;

    }
}

void Ransac::iterRANSAC(int numIterations, double inlierThreshold, Mat &homo) {
    Mat bestHomo;
    int bestInliersCount = 0;
    for (int i = 0; i < numIterations; ++i) {
        //Step 1: Randomly Select 4 pairs of potentially matching point from matches
        int matches_count = good_matches.size();

        //generate 4 random numbers
        //https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
        random_device rd;                                          // obtain a random number from hardware
        mt19937 eng(rd());                                         // seed the generator
        uniform_int_distribution<> distr(0, matches_count - 1);    // define the range

        vector<DMatch> rand_4_matches;
        // generate 4 random numbers
        //TODO: should the 4 random numbers be totally different?
        for (int r = 0; r < 4; ++r) {
            int rnd = distr(eng);
            DMatch rand_match = good_matches[rnd];
            rand_4_matches.push_back(rand_match);
        }

        //Step 2: compute the homography relating the 4 selected matches
        vector<Point2f> obj;
        vector<Point2f> scene;
        for (size_t r = 0; r < rand_4_matches.size(); ++r) {
            obj.push_back(keypoints_vec1[rand_4_matches[r].queryIdx].pt);
            scene.push_back(keypoints_vec2[rand_4_matches[r].trainIdx].pt);
        }
        Mat H = findHomography(obj, scene, 0);

        //compute the number of inliers
        int inliersCount = 0;
        computeInlierCount(H, inliersCount, inlierThreshold);
        if (inliersCount > bestInliersCount) {
            bestInliersCount = inliersCount;
            H.copyTo(bestHomo);
        }
    }
    bestHomo.copyTo(homo);
}

void Ransac::computeRefinedHomography(const Mat &bestHomo, Mat &homo, Mat &homInv, double inlierThreshold) {

    vector<Point2f> obj;
    vector<Point2f> scene;
    for (size_t i = 0; i < good_matches.size(); ++i) {
        //cout << "homo: " << homo << endl;
        DMatch pts_pair = good_matches[i];
        KeyPoint kpt1 = keypoints_vec1[pts_pair.queryIdx];
        KeyPoint kpt2 = keypoints_vec2[pts_pair.trainIdx];

        double x1 = kpt1.pt.x;
        double y1 = kpt1.pt.y;
        double x2 = 0, y2 = 0;

        //compute perspective transform
        project(x1, y1, bestHomo, x2, y2);

        //compute the distance
        double x22 = kpt2.pt.x;
        double y22 = kpt2.pt.y;
        double distance = sqrt(pow((x2 - x22), 2) + pow((y2 - y22), 2));

        if (distance < inlierThreshold)
        {
            obj.push_back(kpt1.pt);
            scene.push_back(kpt2.pt);
            inliers_matches.push_back(pts_pair);
        }
    }
    homo = findHomography(obj, scene, LMEDS);
    homInv = findHomography(scene, obj, LMEDS);
}

void Ransac::displayInliersMatches(const Mat &img1, const Mat &img2) {
    //draw matches
    Mat matches_img;
    drawMatches(img1, keypoints_vec1, img2, keypoints_vec2, inliers_matches, matches_img);
    imshow("Matches", matches_img);
}

void Ransac::doRANSAC(int numIterations, double inlierThreshold, Mat &homo, Mat &homInv) {

    //best homography after many iterations
    Mat bestHomo;
    iterRANSAC(numIterations, inlierThreshold, bestHomo);

    //refined homography
    computeRefinedHomography(bestHomo, homo, homInv, inlierThreshold);
}

