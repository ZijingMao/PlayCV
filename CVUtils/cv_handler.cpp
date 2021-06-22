#include <iostream>
#include "cv_handler.h"
#include "cv_library.h"

using namespace std;
using namespace cv;

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
        int barValue = getTrackbarPos(barName, PROCESSED_NAME);
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
        createTrackbar(barName, PROCESSED_NAME,
                       &params().at(idx).currVal,
                       params().at(idx).maxVal, onBarChange, this);
    }
}
