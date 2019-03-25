#include <string>
#include "ransac.h"
#include "stitch.h"
#include "panorama.h"
#include "functions.h"

using namespace cv;
using namespace std;


int main() {
    //TODO: c++ class big three


    //Panorama
    Panorama *pnrm = new Panorama();
    pnrm->create(5, "Rainier");

    pnrm->stitchToBase(1);
    pnrm->stitchToBase(3);
    pnrm->stitchToBase(4);
    pnrm->stitchToBase(5);
    pnrm->stitchToBase(6);
    pnrm->stitchToBase(2);

//    pnrm->stitchToBase(1);
//    pnrm->stitchToBase(2);
//    pnrm->stitchToBase(3);
//    pnrm->stitchToBase(4);
//    pnrm->stitchToBase(5);
//    pnrm->stitchToBase(6);


    waitKey(0);
    return 0;
}