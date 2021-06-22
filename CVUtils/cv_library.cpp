#include <iostream>
#include "cv_library.h"

using namespace std;
using namespace cv;

CVLibrary::CVLibrary()
{
}

Mat CVLibrary::m_Src;
Mat CVLibrary::m_Dst;
Mat CVLibrary::m_Tmp;

void CVLibrary::onContrastBright(vector<int> &paramValue)
{
    if (paramValue.size() != 2)
    {
        cout << "Method paramter size is: " << paramValue.size() << ", which require to be 2" << endl;
        return;
    }
    m_Src.convertTo(m_Dst, -1, (double)paramValue[0] / 50.0, paramValue[1] - 50);
    imshow(PROCESSED_NAME, m_Dst);
}