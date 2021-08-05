#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cv_manager.h"

using namespace cv;
using namespace std;

int main(int argc, char const *argv[])
{
    // read original image
    string img_path = "D:\\Data\\test.jpg";
    Mat src = imread(img_path, 2);
    resize(src, src, Size(450, 800));
    // if failed to read image
    if (!src.data)
    {
        cout << "Error load images" << endl;
        return -1;
    }
    // create a window
    namedWindow(PROCESSED_WIN, 2);

    vector<string> proc_name = {"ContrastAmplifer"};
    CVManager cvManager = CVManager(src);
    cvManager.initCVLibData();
    cvManager.loadParamMap();
    cvManager.loadCVLibMap();
    cvManager.setProcSeq(proc_name);
    cvManager.loadSequence();
    cvManager.setSrc(src);

    // show the brightness and contrast adjusted image
    imshow(PROCESSED_WIN, cvManager.getDst());
    // wait until user press key for 50ms
    waitKey(0);
    destroyAllWindows();

    return 0;
}