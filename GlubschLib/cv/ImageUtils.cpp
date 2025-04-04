#include "cv/ImageUtils.h"
#include <opencv2/imgproc.hpp>

namespace cv {

Mat fitToSize(const Mat &img, Size size, bool inner)
{
    Mat out;
    fitToSize(img, size, inner, out);
    return out;
}

double fitToSize(const Mat &img, Size size, bool inner, Mat &o_fitImg)
{
    if (img.rows == 0 || img.cols == 0)
        return HUGE_VAL;
    double rowScale = static_cast<double>(size.height) / static_cast<double>(img.rows);
    double colScale = static_cast<double>(size.width) / static_cast<double>(img.cols);
    double scale = inner ? std::min(rowScale, colScale) : std::max(rowScale, colScale);
    if (scale != 1.0)
        resize(img, o_fitImg, Size(0, 0), scale, scale, INTER_AREA);
    else if (img.data != o_fitImg.data)
        img.copyTo(o_fitImg);
    return scale;
}

void showDebug([[maybe_unused]] const Mat &img, [[maybe_unused]] const std::string *pWindowName)
{
#ifndef ANDROID
    std::string windowName = (pWindowName == nullptr) ? "Debug" : *pWindowName;
    namedWindow(windowName, WINDOW_AUTOSIZE);
    if (img.data != nullptr)
    {
        imshow(windowName, img);
        waitKey();
        destroyWindow(windowName);
    }
#endif
}

}    // namespace cv
