#ifndef CV_HANDLER_H
#define CV_HANDLER_H

#include <string>
#include <map>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cv_library.h"

struct UserData
{
    int currVal;
    int maxVal;
    std::string methodName;
    std::string paramName;
};

class CVHandler
{
public:
    CVHandler(std::vector<UserData> &params);
    virtual ~CVHandler() {};

    static cv::Point m_Base;
    static cv::Rect m_Roi;
    static std::vector<cv::Rect> m_RoiSet;
    static cv::Mat m_Layer;
    static cv::Mat m_Working;
    static bool m_bIsDraw;

    static void userData(struct UserData *ud, int cVal, int mVal,
                         std::string &mName, std::string &pName);
    static void onBarChange(int value, void *userData);
    static void onMouseTrigger(int event, int x, int y, int flags, void* userdata);
    void registCVMethod(void (*new_action)(std::vector<int> &paramValue));
    void addTrackBar();
    void onCVMethod();

    std::string &methodName() { return m_sMethodName; }
    std::vector<UserData> &params() { return m_Params; }
    std::vector<int> &paramsVal() { return m_ParamsVal; }

    void methodName(const std::string &methodName)
    {
        m_sMethodName = methodName;
    }
    void updateParamsVal();
    std::map<std::string, void (*)(std::vector<int> &paramValue)> m_Actions;

protected:
    std::string m_sMethodName;
    std::vector<UserData> m_Params;
    std::vector<int> m_ParamsVal;
};

#endif
