#ifndef CV_HANDLER_H
#define CV_HANDLER_H

#include <string>
#include <map>
#include <thread>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cv_library.h"

struct UserData
{
    int currVal;
    int maxVal;
    std::string paramName;
};

class CVHandler
{
public:
    CVHandler();
    virtual ~CVHandler(){};

    static cv::Point m_Base;
    static cv::Rect m_Roi;
    static std::vector<cv::Rect> m_RoiSet;
    static cv::Mat m_Layer;
    static cv::Mat m_Working;
    static bool m_bIsDraw;

    static void userData(struct std::shared_ptr<UserData> &ud, int cVal, int mVal, std::string &pName);
    static void onBarChange(int value, void *userData);
    static void onMouseTrigger(int event, int x, int y, int flags, void *userdata);
    void registCVMethod(std::string &mName,
                        void (*new_action)(cv::Mat &src, cv::Mat &dst, std::vector<int> &paramValue));
    void registCVParams(std::string &mName, std::vector<std::shared_ptr<UserData>> &data);
    void registCVImgs(std::string &mName, cv::Mat &src, cv::Mat &dst);
    void addTrackBar();
    void onCVMethod();
    void updateParamsVal();

    std::map<std::string, void (*)(cv::Mat &src, cv::Mat &dst, std::vector<int> &paramValue)> m_Actions;
    std::map<std::string, std::vector<std::shared_ptr<UserData>>> m_Params;
    std::map<std::string, std::vector<int>> m_ParamsVal;
    std::map<std::string, cv::Mat> m_SrcImgs;
    std::map<std::string, cv::Mat> m_DstImgs;
    std::vector<std::string> m_sMethodName;
};

#endif
