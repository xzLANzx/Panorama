#include <string>
#include "panorama.h"

using namespace cv;
using namespace std;


int main(int argc, char *argv[]) {

    int img_count = stoi(argv[1]);
    string img_name = argv[2];

    auto *pnrm = new Panorama();
    pnrm->stitchAll(img_count, img_name);

    waitKey(0);
    return 0;
}