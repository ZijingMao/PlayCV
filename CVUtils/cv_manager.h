#ifndef CV_MANAGER_H
#define CV_MANAGER_H

#include <string>
#include <map>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cv_handler.h"
#include "cv_library.h"

class CVManager
{
public:
    CVManager(std::string imgPath);
    CVManager(cv::Mat src);
    CVManager(cv::Mat src, cv::Mat tmp);
    virtual ~CVManager(){};

    void loadParamMap();
    void loadSequence();
    void loadCVLibMap();
    void initCVLibData();
    void setSrc(cv::Mat src)
    {
        m_Src = src;
        m_SrcList[0] = src;
        CVLibrary::m_Src = m_Src;
    }
    void setDst(cv::Mat dst)
    {
        m_Dst = dst;
        CVLibrary::m_Dst = m_Dst;
    }
    void setTmp(cv::Mat tmp)
    {
        m_Tmp = tmp;
        CVLibrary::m_Tmp = m_Tmp;
    }
    void setProcSeq(std::vector<std::string> procSeq) { m_ProcessSequence = procSeq; }

    cv::Mat getSrc() { return m_Src; }
    cv::Mat getDst() { return m_Dst; }
    cv::Mat getTmp() { return m_Tmp; }

    std::vector<std::vector<std::shared_ptr<UserData>>> m_uDataVector;
    std::vector<cv::Mat> m_SrcList;
    CVHandler m_CVHandler;
protected:
    cv::Mat m_Src;
    cv::Mat m_Dst;
    cv::Mat m_Tmp;
    std::map<std::string, std::vector<std::string>> m_M2P;
    std::vector<std::string> m_ProcessSequence;
    std::string m_sImgPath;
    std::map<std::string, void (*)(cv::Mat &src, cv::Mat &dst, std::vector<int> &params)> m_CVLibMap;
};

#endif