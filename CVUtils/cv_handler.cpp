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

CVHandler::CVHandler(vector<UserData> &params) : m_Params(params)
{
    if (params.size() > 0)
    {
        methodName(params[0].methodName);
    }
}

void CVHandler::onCVMethod()
{
    if (m_Actions[methodName()])
    {
        updateParamsVal();
        m_Actions[methodName()](paramsVal());
    }
}

void CVHandler::registCVMethod(void (*new_action)(vector<int> &paramValue))
{
    m_Actions[methodName()] = new_action;
}

void CVHandler::onBarChange(int value, void *userData)
{
    CVHandler *c = (CVHandler *)(userData);
    string methodName = c->methodName();
    c->onCVMethod();
}

void CVHandler::updateParamsVal()
{
    m_ParamsVal.clear();
    for (int idx = 0; idx < params().size(); idx++)
    {
        string barName = methodName() + "-" + params().at(idx).paramName;
        int barValue = getTrackbarPos(barName, PROCESSED_WIN);
        m_ParamsVal.emplace_back(barValue);
        cout << barName << ":\t" << barValue << endl;
    }
}

void CVHandler::userData(UserData *ud, int cVal, int mVal,
                         string &mName, string &pName)
{
    ud->currVal = cVal;
    ud->maxVal = mVal;
    ud->methodName = mName;
    ud->paramName = pName;
}

void CVHandler::addTrackBar()
{
    for (int idx = 0; idx < params().size(); idx++)
    {
        string barName = methodName() + "-" + params().at(idx).paramName;
        createTrackbar(barName, PROCESSED_WIN,
                       &params().at(idx).currVal,
                       params().at(idx).maxVal, onBarChange, this);
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
