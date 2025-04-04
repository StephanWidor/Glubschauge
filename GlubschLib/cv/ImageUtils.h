#pragma once
#include <opencv2/highgui.hpp>

namespace cv {

double fitToSize(const Mat &img, Size size, bool inner, Mat &o_fitImg);

cv::Mat fitToSize(const Mat &img, Size size, bool inner);

/// Shows image on Desktop until any key pressed, does nothing on Android
void showDebug([[maybe_unused]] const Mat &img, [[maybe_unused]] const std::string *pWindowName = nullptr);

}    // namespace cv
