#include "harrisCorner.h"
void loadImage(Mat &img, string imgName){

    img = imread(imgName, IMREAD_COLOR);

}

int main() {

    //load image
    Mat img;
    string imgName = "../project_images/Boxes.png";
    loadImage(img, imgName);

    //compute corner strength matrix
    Mat corner_strength_mat;
    computeCornerStrengthMat(img, corner_strength_mat);

    //get all the key points
    vector<KeyPoint> keypoints_vec;
    getKeypointsFromCornerStrengthMat(corner_strength_mat, keypoints_vec);

    //display all the key points
    displayKeypoints(keypoints_vec, img);

    waitKey(0);
    return 0;
}