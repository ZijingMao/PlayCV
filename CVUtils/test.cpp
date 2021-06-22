#include <iostream>
#include <map>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cv_manager.h"

using namespace std;
using namespace cv;

int main(int argc, char const *argv[])
{
    // read original image
    string img_path = "D:\\Data\\test.bmp";
    Mat src = imread(img_path);
    resize(src, src, Size(612, 512));
    // if failed to read image
    if (!src.data)
    {
        cout << "Error load images" << endl;
        return -1;
    }
    // create a window
    namedWindow(PROCESSED_NAME, 2);

    vector<string> proc_name = {"ContrastBright", "test"};
    CVManager cvManager = CVManager(src);
    cvManager.loadParamMap();
    cvManager.loadCVLibMap();
    cvManager.setProcSeq(proc_name);
    cvManager.loadSequence();

    // show the brightness and contrast adjusted image
    imshow(PROCESSED_NAME, cvManager.getDst());
    // wait until user press key for 50ms
    waitKey(0);
    destroyAllWindows();

    return 0;
}