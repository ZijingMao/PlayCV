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
Mat CVLibrary::m_Prev;

array<array<float, 4>, 256> CVLibrary::m_vColorTable;

void CVLibrary::onContrastBright(vector<int> &paramValue)
{
    if (paramValue.size() != 2)
    {
        cout << "Method paramter size is: " << paramValue.size() << ", which require to be 2" << endl;
        return;
    }
    m_Src.convertTo(m_Dst, -1, (double)paramValue[1] / 50.0, paramValue[0] - 50);
    imshow(PROCESSED_WIN, m_Dst);
}

void CVLibrary::makeColorWheel(vector<Scalar> &colorwheel)
{
    int RY = 15;
    int YG = 6;
    int GC = 4;
    int CB = 11;
    int BM = 13;
    int MR = 6;

    int i;
    for (i = 0; i < RY; i++)
        colorwheel.push_back(Scalar(255, 255 * i / RY, 0));
    for (i = 0; i < YG; i++)
        colorwheel.push_back(Scalar(255 - 255 * i / YG, 255, 0));
    for (i = 0; i < GC; i++)
        colorwheel.push_back(Scalar(0, 255, 255 * i / GC));
    for (i = 0; i < CB; i++)
        colorwheel.push_back(Scalar(0, 255 - 255 * i / CB, 255));
    for (i = 0; i < BM; i++)
        colorwheel.push_back(Scalar(255 * i / BM, 0, 255));
    for (i = 0; i < MR; i++)
        colorwheel.push_back(Scalar(255, 0, 255 - 255 * i / MR));
}

void CVLibrary::motionToColor(Mat flow, Mat &color)
{
    if (flow.empty())
    {
        cout << "Motion to color aquired null image." << endl;
        return;
    }
    if (color.empty())
        color.create(flow.rows, flow.cols, CV_8UC3);

    static vector<Scalar> colorwheel; //Scalar r,g,b
    if (colorwheel.empty())
        makeColorWheel(colorwheel);

    // determine motion range:
    float maxrad = -1;

    // Find max flow to normalize fx and fy
    for (int i = 0; i < flow.rows; ++i)
    {
        for (int j = 0; j < flow.cols; ++j)
        {
            Vec2f flow_at_point = flow.at<Vec2f>(i, j);
            float fx = flow_at_point[0];
            float fy = flow_at_point[1];
            if ((fabs(fx) > UNKNOWN_FLOW_THRESH) || (fabs(fy) > UNKNOWN_FLOW_THRESH))
                continue;
            float rad = sqrt(fx * fx + fy * fy);
            maxrad = maxrad > rad ? maxrad : rad;
        }
    }
    // normalization
    for (int i = 0; i < flow.rows; ++i)
    {
        for (int j = 0; j < flow.cols; ++j)
        {
            uchar *data = color.data + color.step[0] * i + color.step[1] * j;
            Vec2f flow_at_point = flow.at<Vec2f>(i, j);

            float fx = flow_at_point[0] / maxrad;
            float fy = flow_at_point[1] / maxrad;
            if ((fabs(fx) > UNKNOWN_FLOW_THRESH) || (fabs(fy) > UNKNOWN_FLOW_THRESH))
            {
                data[0] = data[1] = data[2] = 0;
                continue;
            }
            float rad = sqrt(fx * fx + fy * fy);
            float angle = atan2(-fy, -fx) / CV_PI;
            float fk = (angle + 1.0) / 2.0 * (colorwheel.size() - 1);
            int k0 = (int)fk;
            int k1 = (k0 + 1) % colorwheel.size();
            float f = fk - k0;

            for (int b = 0; b < 3; b++)
            {
                float col0 = colorwheel[k0][b] / 255.0;
                float col1 = colorwheel[k1][b] / 255.0;
                float col = (1 - f) * col0 + f * col1;
                if (rad <= 1)
                    col = 1 - rad * (1 - col); // increase saturation with radius
                else
                    col *= .75; // out of range
                data[2 - b] = (int)(255.0 * col);
            }
        }
    }
}

void CVLibrary::onOpticalFlow(vector<int> &paramValue)
{
    if (paramValue.size() != 3)
    {
        cout << "Method paramter size is: " << paramValue.size() << ", which require to be 3" << endl;
        return;
    }
    int levels, winsize, iterations;
    levels = paramValue[0] / 10 + 1;    // range from [1:1:11]
    winsize = paramValue[1] / 5 + 1;    // range from [1:1:21]
    iterations = paramValue[2] / 5 + 1; // range from [1:1:21]
    Mat flow;
    if (m_Src.channels() != 1)
        cvtColor(m_Src, m_Tmp, CV_BGR2GRAY);
    if (m_Prev.data)
    {
        calcOpticalFlowFarneback(m_Prev, m_Tmp, flow, 0.5, levels, winsize, iterations, 5, 1.2, 0);
        motionToColor(flow, m_Dst);
        imshow(PROCESSED_WIN, m_Dst);
    }
    std::swap(m_Prev, m_Tmp);
}

void CVLibrary::makeColorRatio(const vector<Rect> &roiRegions)
{
    if (roiRegions.empty())
    {
        cout << "no region specified to make color ratio" << endl;
        return;
    }
    // init table
    for (int i = 0; i < 255; i++)
    {
        m_vColorTable[i][0] = INFINITY;
        m_vColorTable[i][2] = INFINITY;
    }
    // you can save the data and load when the program init
    for (const Rect &r : roiRegions)
    {
        Mat roi = m_Src(r); // get the cropped roi region
        Mat roi_gray;
        cvtColor(roi, roi_gray, CV_BGR2GRAY);
        for (int i = 0; i < roi.rows; i++)
        {
            for (int j = 0; j < roi.cols; j++)
            {
                int b = roi.at<Vec3b>(i, j)[0];
                int g = roi.at<Vec3b>(i, j)[1];
                int r = roi.at<Vec3b>(i, j)[2];
                float b2g = (float)(b * 1.0 / g);
                float g2r = (float)(g * 1.0 / r);
                int gs = roi_gray.at<uchar>(i, j);
                if (gs > 255 || gs < 0)
                {
                    cout << "gray scale error not range in 0-255" << endl;
                    return;
                }
                if (m_vColorTable[gs][0] > b2g)
                    m_vColorTable[gs][0] = b2g;
                if (m_vColorTable[gs][1] < b2g)
                    m_vColorTable[gs][1] = b2g;
                if (m_vColorTable[gs][2] > g2r)
                    m_vColorTable[gs][2] = g2r;
                if (m_vColorTable[gs][3] < g2r)
                    m_vColorTable[gs][3] = g2r;
            }
        }
    }
}

void CVLibrary::onColorRatioSegment(vector<int> &paramValue)
{
    if (paramValue.size() != 1)
    {
        cout << "Method paramter size is: " << paramValue.size() << ", which require to be 3" << endl;
        return;
    }
    if (m_vColorTable.size() != 256)
    {
        cout << "make color ratio incorrect: " << m_vColorTable.size() << endl;
        return;
    }
    float loose = paramValue[0] / 100.0; // range from [0:0.01:1]
    float b2g(0), g2r(0);
    m_Dst = Mat::zeros(m_Src.rows, m_Src.cols, CV_8UC1);
    Mat gray;
    cvtColor(m_Src, gray, CV_BGR2GRAY);
    for (int i = 0; i < m_Src.rows; i++)
    {
        for (int j = 0; j < m_Src.cols; j++)
        {
            int b = m_Src.at<Vec3b>(i, j)[0];
            int g = m_Src.at<Vec3b>(i, j)[1];
            int r = m_Src.at<Vec3b>(i, j)[2];
            b2g = (float)(b * 1.0 / g);
            g2r = (float)(g * 1.0 / r);
            int gs = gray.at<uchar>(i, j);
            if (b2g >= m_vColorTable[gs][0] - loose &&
                b2g <= m_vColorTable[gs][1] + loose &&
                g2r >= m_vColorTable[gs][2] - loose &&
                g2r <= m_vColorTable[gs][3] + loose)
            {
                m_Dst.at<uchar>(i, j) = 255;
            }
        }
    }
    imshow(PROCESSED_WIN, m_Dst);
}
