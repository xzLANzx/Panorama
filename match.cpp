//
// Created by ZelanXiao on 2019/3/15.
//

#include "match.h"
Match::Match() {
}

void Match::normalizeFeatureDescriptors(const Mat &descriptors, Mat &norm_descriptors) {
    norm_descriptors.release();
    for (size_t i = 0; i < descriptors.size().height; ++i) {
        Mat descriptor_128 = descriptors.row(i);
        Mat norm_descriptor_128;

        //normalize 128-d feature descriptor
        normalize(descriptor_128, norm_descriptor_128, 1.0, 0.0, NORM_L2);

        //clamp to 0.2
        threshold(norm_descriptor_128, norm_descriptor_128, 0.2, 0.2, 2);

        //normalize again
        normalize(norm_descriptor_128, norm_descriptor_128, 0, 1, NORM_MINMAX);

        norm_descriptors.push_back(norm_descriptor_128);
    }
}

void Match::computeNormSIFTDescriptors(const Mat &gray_img, vector<KeyPoint> &keypoints_vec, Mat &descriptors_vec) {

    /*
    //harris corner keypoints detection
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;
    HarrisCornerDetection *harrisKeypointsDetector = new HarrisCornerDetection();
    harrisKeypointsDetector->computeHarrisResponse(gray_img, blockSize, apertureSize, k);
    harrisKeypointsDetector->detectKeyPoints(keypoints_vec, 80);
     */


    Ptr<SIFT> sift = SIFT::create(0, 5, 0.04, 10, 1.6);
    //sift keypoints detection
    sift->detect(gray_img, keypoints_vec);

    //compute sift feature descriptors
    sift->compute(gray_img, keypoints_vec, descriptors_vec);

    //normalize the sift feature descriptors
    Mat norm_descriptors_vec;
    normalizeFeatureDescriptors(descriptors_vec, norm_descriptors_vec);
    norm_descriptors_vec.copyTo(descriptors_vec);
}

void Match::SSDRatioTest(const vector<vector<DMatch>> &matches, float ratio, float thresh) {

    for (size_t i = 0; i < matches.size(); ++i) {
        DMatch match1 = matches[i][0];
        DMatch match2 = matches[i][1];
        //cout << i << ": " << match1.distance << "\t" << match2.distance << endl;
        if ((match1.distance < (ratio * match2.distance)) && (match1.distance < thresh)) {
            good_matches.push_back(match1);
        }
    }
}

void Match::findMatches(const Mat &gray_img1, const Mat &gray_img2) {

    //compute all the keypoints and their feature descriptors
    computeNormSIFTDescriptors(gray_img1, keypoints_vec1, descriptors_vec1);
    computeNormSIFTDescriptors(gray_img2, keypoints_vec2, descriptors_vec2);

    //match descriptor vectors using BFMatcher
    BFMatcher matcher;
    vector<vector<DMatch>> matches;
    matcher.knnMatch(descriptors_vec1, descriptors_vec2, matches, 2);

    //perform SSD ratio test
    float ratio = 0.75;
    float thresh = 2.5;
    SSDRatioTest(matches, ratio, thresh);

    /*
    //draw matches
    Mat matches_img;
    drawMatches(gray_img1, keypoints_vec1, gray_img2, keypoints_vec2, good_matches, matches_img);
    imshow("Matches", matches_img);
    */
}

vector<KeyPoint> Match::getKeypointsVec(int i){
    if(i == 1)
        return keypoints_vec1;
    else if(i == 2)
        return keypoints_vec2;
    else
        exit(1);
}

Mat Match::getFeatureDescriptors(int i){
    if(i == 1)
        return descriptors_vec1;
    else if(i == 2)
        return descriptors_vec2;
    else
        exit(1);
}

vector<DMatch> Match::getMatches(){
    return good_matches;
}