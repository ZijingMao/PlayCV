#include <iostream>
#include "cv_handler.h"
#include "cv_library.h"

using namespace std;
using namespace cv;

Point CVHandler::m_Base;
Rect CVHandler::m_Roi;
vector<Rect> CVHandler::m_RoiSet;
Mat CVHandler::m_Layer;
Mat CVHandler::m_Working;
bool CVHandler::m_bIsDraw = false;

CVHandler::CVHandler()
{
}

void CVHandler::onCVMethod()
{
    for (auto method : m_sMethodName)
    {
        if (m_Actions[method])
        {
            updateParamsVal();
            m_Actions[method](*m_SrcImgs[method],
                              *m_DstImgs[method],
                              m_ParamsVal[method]);
            
        }
    }
    Mat dst = *m_DstImgs[m_sMethodName.back()];
    if (dst.data)
        imshow(PROCESSED_WIN, dst);
}

void CVHandler::registCVMethod(string &mName, void (*new_action)(Mat &src, Mat &dst, vector<int> &paramValue))
{
    m_Actions[mName] = new_action;
}

void CVHandler::registCVParams(string &mName, vector<shared_ptr<UserData>> &data)
{
    m_Params[mName] = data;
}

void CVHandler::registCVImgs(string &mName, Mat &src, Mat &dst)
{
    m_SrcImgs[mName] = &src;
    m_DstImgs[mName] = &dst;
}

void CVHandler::onBarChange(int value, void *userData)
{
    CVHandler *c = (CVHandler *)(userData);
    c->onCVMethod();
}

void CVHandler::updateParamsVal()
{
    for (auto method : m_sMethodName)
    {
        m_ParamsVal[method].clear();
        for (int idx = 0; idx < m_Params[method].size(); idx++)
        {
            string barName = method + "-" + m_Params[method].at(idx)->paramName;
            int barValue = getTrackbarPos(barName, PROCESSED_WIN);
            m_ParamsVal[method].emplace_back(barValue);
            cout << barName << ":\t" << barValue << endl;
        }
    }
}

void CVHandler::userData(shared_ptr<UserData> &ud, int cVal, int mVal, string &pName)
{
    ud->currVal = cVal;
    ud->maxVal = mVal;
    ud->paramName = pName;
}

void CVHandler::addTrackBar()
{
    for (auto method : m_sMethodName)
    {
        for (int idx = 0; idx < m_Params[method].size(); idx++)
        {
            string barName = method + "-" + m_Params[method].at(idx)->paramName;
            createTrackbar(barName, PROCESSED_WIN,
                           &m_Params[method].at(idx)->currVal,
                           m_Params[method].at(idx)->maxVal, onBarChange, this);
        }
    }
}

void CVHandler::onMouseTrigger(int event, int x, int y, int flags, void *userdata)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        m_Base.x = x;
        m_Base.y = y;
        m_Roi.x = x;
        m_Roi.y = y;
        m_Roi.width = 0;
        m_Roi.height = 0;
        m_bIsDraw = true;
    }
    else if (event == EVENT_MOUSEMOVE)
    {
        if (!m_bIsDraw)
            return;

        m_Roi.width = abs(m_Roi.x - x);
        m_Roi.height = abs(m_Roi.y - y);
        if (x < m_Base.x)
        {
            m_Roi.x = x;
            m_Roi.width = abs(m_Base.x - x);
        }
        if (y < m_Base.y)
        {
            m_Roi.y = y;
            m_Roi.height = abs(m_Base.y - y);
        }
        m_Working = m_Layer.clone();
        rectangle(m_Working, m_Roi, Scalar(0, 255, 0));
        imshow(DRAW_RECT_WIN, m_Working);
    }
    else if (event == EVENT_LBUTTONUP)
    {
        if (m_Roi.width == 0 || m_Roi.height == 0)
            return;
        // show the rectangle coordinates
        cout << "Rect: " << m_Roi.x << "\t" << m_Roi.y
             << "\t" << m_Roi.width << "\t" << m_Roi.height << endl;
        m_RoiSet.emplace_back(m_Roi);
        m_Roi = Rect();
        m_bIsDraw = false;
        m_Working = m_Layer.clone();
        rectangle(m_Working, m_Roi, Scalar(0, 255, 0));
        imshow(DRAW_RECT_WIN, m_Working);
    }
}
