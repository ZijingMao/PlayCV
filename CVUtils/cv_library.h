#ifndef CV_LIBRARY_H
#define CV_LIBRARY_H

#include <string>
#include <map>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#define PROCESSED_WIN "PROCESSED"
#define DRAW_RECT_WIN "Rectangle Acquired"
#define UNKNOWN_FLOW_THRESH 1e9

class CVLibrary
{
public:
    CVLibrary();
    virtual ~CVLibrary(){};

    static cv::Mat m_Src;
    static cv::Mat m_Dst;
    static cv::Mat m_Tmp;
    static cv::Mat m_Prev;

    static std::array<std::array<float, 4>, 256> m_vColorTable;

    static void onContrastBright(std::vector<int> &paramValue);
    static void makeColorWheel(std::vector<cv::Scalar> &colorwheel);
    static void motionToColor(cv::Mat flow, cv::Mat &color);
    static void onOpticalFlow(std::vector<int> &paramValue);
    static void makeColorRatio(const std::vector<cv::Rect> &roiRegions);
    static void onColorRatioSegment(std::vector<int> &paramValue);
};

#endif