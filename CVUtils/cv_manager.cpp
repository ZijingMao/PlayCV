#include <string>
#include <iostream>
#include <thread>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cv_manager.h"

using namespace std;
using namespace cv;

CVManager::CVManager(Mat src) : m_Src(src)
{
    setDst(src.clone());
    setTmp(src.clone());
}

CVManager::CVManager(Mat src, Mat tmp) : m_Src(src), m_Tmp(tmp)
{
    setDst(src.clone());
}

void CVManager::loadParamMap()
{
    m_M2P = {
        {"ContrastBright", {"bright", "contrast"}},
        {"OpticalFlow", {"levels", "winsize", "iterations"}},
        {"ColorRatioSegment", {"loose"}},
        {"MedianBlur", {"kernel"}},
        {"MorphologyEx", {"kernel"}}};
}

void CVManager::loadCVLibMap()
{
    m_CVLibMap.insert(make_pair("ContrastBright", CVLibrary::onContrastBright));
    m_CVLibMap.insert(make_pair("OpticalFlow", CVLibrary::onOpticalFlow));
    m_CVLibMap.insert(make_pair("ColorRatioSegment", CVLibrary::onColorRatioSegment));
    m_CVLibMap.insert(make_pair("MedianBlur", CVLibrary::onMedianBlur));
    m_CVLibMap.insert(make_pair("MorphologyEx", CVLibrary::onMorphologyEx));
}

void CVManager::initCVLibData()
{
    CVLibrary::m_Src = getSrc();
    CVLibrary::m_Dst = getDst();
    CVLibrary::m_Tmp = getTmp();
}

void CVManager::loadSequence()
{
    int processCount = m_ProcessSequence.size();
    m_SrcList.clear();
    m_uDataVector.clear();
    // reset img list to given length
    m_SrcList.resize(processCount + 1);  
    // the first img is stored by src img 
    m_SrcList[0] = CVLibrary::m_Src;
    // reset vector of user data list with given length
    m_uDataVector.resize(processCount);
    for (int idx = 0; idx < processCount; idx++)
    {
        string m = m_ProcessSequence[idx];
        // add to method name of cv handler
        m_CVHandler.m_sMethodName.emplace_back(m);
        vector<string> p = m_M2P[m];
        for (int idy = 0; idy < p.size(); idy++)
        {
            shared_ptr<UserData> u = make_shared<UserData>();
            // also save the pointer of image sequences used for next processor
            CVHandler::userData(u, 50, 100, p[idy]);
            m_uDataVector[idx].emplace_back(u);
        }
        // TODO: dynamic pointer cast to smart point
        m_CVHandler.registCVMethod(m, m_CVLibMap[m]);
        m_CVHandler.registCVParams(m, m_uDataVector[idx]);
        m_CVHandler.registCVImgs(m, m_SrcList[idx], m_SrcList[idx + 1]);
    }
    m_CVHandler.addTrackBar();
}
