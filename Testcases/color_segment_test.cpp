#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cv_manager.h"

using namespace cv;
using namespace std;

int main(int argc, char const *argv[])
{
    // read original image
    string img_path = "D:\\Data\\test.bmp";
    Mat src = imread(img_path);
    resize(src, src, Size(612, 512));
    CVHandler::m_Layer = src.clone();
    CVHandler::m_Working = src.clone();
    CVHandler::m_bIsDraw = false;
    // if failed to read image
    if (!src.data)
    {
        cout << "Error load images" << endl;
        return -1;
    }
    // create a window
    namedWindow(PROCESSED_WIN, 2);
    namedWindow(DRAW_RECT_WIN, 2);

    imshow(DRAW_RECT_WIN, CVHandler::m_Working);
    setMouseCallback(DRAW_RECT_WIN, CVHandler::onMouseTrigger, NULL);
    // wait until key pressed
    waitKey(0);

    vector<string> proc_name = {"ColorRatioSegment"};
    CVManager cvManager = CVManager(src);
    cvManager.initCVLibData();
    CVLibrary::makeColorRatio(CVHandler::m_RoiSet);
    cvManager.loadParamMap();
    cvManager.loadCVLibMap();
    cvManager.setProcSeq(proc_name);
    cvManager.loadSequence();

    // show the brightness and contrast adjusted image
    imshow(PROCESSED_WIN, cvManager.getDst());
    // wait until user press key for 50ms
    waitKey(0);
    destroyAllWindows();

    return 0;
}