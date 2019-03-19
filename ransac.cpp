//
// Created by ZelanXiao on 2019/3/13.
//

#include "ransac.h"

namespace self {
    RANSAC::RANSAC() {
    }

    void RANSAC::create(const Mat &gray_img1, const Mat &gray_img2) {
        Match *matcher = new Match();
        matcher->findMatches(gray_img1, gray_img2);
        keypoints_vec1 = matcher->getKeypointsVec(1);
        keypoints_vec2 = matcher->getKeypointsVec(2);
        good_matches = matcher->getMatches();
    }

    void RANSAC::project(const double &x1, const double &y1, const Mat &homo, double &x2, double &y2) {
        Mat src_pt = Mat::zeros(3, 1, homo.type());

        src_pt.at<double>(0, 0) = x1;
        src_pt.at<double>(1, 0) = y1;
        src_pt.at<double>(2, 0) = 1;


        Mat dst_pt = homo * src_pt;
        double w = dst_pt.at<double>(2, 0);

        x2 = dst_pt.at<double>(0, 0) /= w;
        y2 = dst_pt.at<double>(1, 0) /= w;
    }

    //computes the number of inlying points given a homography H
    void
    RANSAC::computeInlierCount(const Mat &homo, int &inliersCount, double inlierThreshold) {
        for (size_t i = 0; i < good_matches.size(); ++i) {
            //cout << "homo: " << homo << endl;
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

//            cout << "(x1, y1): (" << x1 << ", " << y1 << ")\t(x2, y2): (" << x2 << ", " << y2 << ")\t(x22, y22): ("
//                 << x22 << ", " << y22 << ")" << endl;
            //cout << "Distance: " << distance << endl;
            if (distance < inlierThreshold)
                inliersCount++;

        }
    }

    void RANSAC::iterRANSAC(int numIterations, double inlierThreshold, Mat &homo) {
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
            //cout << "H: " << H << endl;

            //compute the number of inliers
            int inliersCount = 0;
            computeInlierCount(H, inliersCount, inlierThreshold);
            if (inliersCount > bestInliersCount) {
                bestInliersCount = inliersCount;
                H.copyTo(bestHomo);
            }

//            cout << "Total number of matches: " << matches_count << endl;
//            cout << "Max Inliers Count: " << bestInliersCount << endl;
//            cout << "Best Homography: " << bestHomo << endl;
//            cout << endl;
        }
        bestHomo.copyTo(homo);
    }

    void RANSAC::computeRefinedHomography(const Mat &bestHomo, Mat &homo, Mat &homInv, double inlierThreshold) {

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

    void RANSAC::homographyTransform(const Mat &src_img, const Mat &homo, Mat &out_img) {

        out_img = Mat::zeros(src_img.size(), src_img.type());
        for (int i = 0; i < src_img.rows; ++i) {
            for (int j = 0; j < src_img.cols; ++j) {
                double x1 = j, y1 = i;
                double x2 = 0, y2 = 0;
                project(x1, y1, homo, x2, y2);

                //mark sure it's not going out of boundary
                if (x2 >= 0 && x2 < src_img.cols && y2 >= 0 && y2 < src_img.rows) {
                    out_img.at<Vec3b>(y2, x2) = src_img.at<Vec3b>(y1, x1);
                }
            }
        }
        imshow("Result", out_img);
    }

    void RANSAC::homographyTransform2(const Mat &src_img, const Mat &homo, Mat &out_img) {

        vector<Point2f> src_pts_vec;
        for (int i = 0; i < src_img.rows; ++i) {
            for (int j = 0; j < src_img.cols; ++j) {
                src_pts_vec.push_back(Point2f(j, i));
            }
        }

        vector<Point2f> dst_pts_vec;
        perspectiveTransform(src_pts_vec, dst_pts_vec, homo);

        out_img = Mat::zeros(src_img.size(), src_img.type());
        for (size_t i = 0; i < src_pts_vec.size(); ++i) {

            float x1 = src_pts_vec[i].x;
            float y1 = src_pts_vec[i].y;

            float x2 = dst_pts_vec[i].x;
            float y2 = dst_pts_vec[i].y;

            //mark sure it's not going out of boundary
            if (x2 >= 0 && x2 < src_img.cols && y2 >= 0 && y2 < src_img.rows) {
                out_img.at<Vec3b>(y2, x2) = src_img.at<Vec3b>(y1, x1);
            }
        }
        imshow("Result2", out_img);
    }

    void RANSAC::displayInliersMatches(const Mat &img1, const Mat &img2) {
        //draw matches
        Mat matches_img;
        drawMatches(img1, keypoints_vec1, img2, keypoints_vec2, inliers_matches, matches_img);
        imshow("Matches", matches_img);
    }

    void RANSAC::doRANSAC(int numIterations, double inlierThreshold, Mat &homo, Mat &homInv,
                          const Mat &displayImage1, const Mat &displayImage2) {

        //best homography after many iterations
        Mat bestHomo;
        iterRANSAC(numIterations, inlierThreshold, bestHomo);

        //refined homography
        computeRefinedHomography(bestHomo, homo, homInv, inlierThreshold);

        //display inliers
        displayInliersMatches(displayImage1, displayImage2);

    }
}
