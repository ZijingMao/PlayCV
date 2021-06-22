#include <string>
#include <iostream>
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
        {"test", {"bar"}}};
}

void CVManager::loadCVLibMap()
{
    m_CVLibMap.insert(make_pair("ContrastBright", CVLibrary::onContrastBright));
    m_CVLibMap.insert(make_pair("test", CVLibrary::onContrastBright));
}

void CVManager::loadSequence()
{
    CVLibrary::m_Src = m_Src;
    CVLibrary::m_Dst = m_Dst;
    CVLibrary::m_Tmp = m_Tmp;
    vector<vector<UserData>> uDataVector;
    vector<CVHandler> cMethod;
    for (int idx = 0; idx < m_ProcessSequence.size(); idx++)
    {
        string m = m_ProcessSequence[idx];
        vector<string> p = m_M2P[m];
        vector<UserData> *uData = new vector<UserData>();
        for (int idy = 0; idy < p.size(); idy++)
        {
            UserData *u = new UserData();
            CVHandler::userData(u, 50, 100, m, p[idy]);
            uData->emplace_back(*u);
        }
        CVHandler *c = new CVHandler(*uData);
        cMethod.emplace_back(*c);
        uDataVector.emplace_back(*uData);
        c->addTrackBar();
        c->registCVMethod(m_CVLibMap[m]);
    }
}
