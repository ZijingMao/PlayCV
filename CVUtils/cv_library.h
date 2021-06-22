#ifndef CV_LIBRARY_H
#define CV_LIBRARY_H

#include <string>
#include <map>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#define PROCESSED_NAME "PROCESSED"

class CVLibrary
{
public:
    CVLibrary();
    virtual ~CVLibrary(){};

    static cv::Mat m_Src;
    static cv::Mat m_Dst;
    static cv::Mat m_Tmp;

    static void onContrastBright(std::vector<int> &paramValue);
};

#endif