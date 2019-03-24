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
    pnrm->create(6, "desk");

    pnrm->stitch();
    pnrm->stitch();
    pnrm->stitch();
    pnrm->stitch();
    pnrm->stitch();

    pnrm->printMatchesCount();

    waitKey(0);
    return 0;
}