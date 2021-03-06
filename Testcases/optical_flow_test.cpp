#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cv_manager.h"

using namespace cv;
using namespace std;

int main(int argc, char const *argv[])
{
    VideoCapture cap;
    cap.open(0);

    if (!cap.isOpened())
        return -1;
    Mat src;
    // create a window
    namedWindow(PROCESSED_WIN, 2);

    vector<string> proc_name = {"OpticalFlow"};
    CVManager cvManager = CVManager(src);
    cvManager.initCVLibData();
    cvManager.loadParamMap();
    cvManager.loadCVLibMap();
    cvManager.setProcSeq(proc_name);
    cvManager.loadSequence();

    while (true)
    {
        cap >> src;
        cvManager.setSrc(src);
        imshow("original", src);
        cvManager.m_CVHandler.onCVMethod();
        char c = waitKey(50);
        if (c == 27)
            break;
    }
    destroyAllWindows();

    return 0;
}