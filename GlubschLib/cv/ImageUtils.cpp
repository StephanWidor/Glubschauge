#include "cv/ImageUtils.h"
#include "cv/Utils2D.h"
#include <omp.h>

namespace cv {

Mat ImageUtils::fitToSize(const Mat &img, Size size, bool inner)
{
    Mat out;
    fitToSize(img, size, inner, out);
    return out;
}

double ImageUtils::fitToSize(const Mat &img, Size size, bool inner, Mat &o_fitImg)
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

void ImageUtils::distort(Mat &io_img, const BarrelInfo &barrelInfo)
{
    assert(io_img.type() == CV_8UC3);
    if (barrelInfo.radius <= 0.0 || barrelInfo.power <= 0.0)
        return;

    const auto infoRect = boundingRect(barrelInfo) & cv::Rect(0, 0, io_img.cols - 1, io_img.rows - 1);
    const auto tlInfo = infoRect.tl();
    const auto brInfo = infoRect.br();
    Mat infoImg = io_img(infoRect).clone();

    // OpenCV memory order is row first, so parallelizing row processing should be better than columns
    const auto processRow = [&](int row) {
        for (int col = tlInfo.x; col <= brInfo.x; ++col)
        {
            Point point(col, row);
            const auto diff = point - barrelInfo.center;
            double dist = norm(diff);
            if (dist == 0.0 || dist >= barrelInfo.radius)
                continue;
            double distRatio = std::pow(dist / barrelInfo.radius, barrelInfo.power);
            Point readPoint = barrelInfo.center + distRatio * diff;
            if (readPoint == point)
                continue;
            infoImg.at<Vec3b>(point - tlInfo) = io_img.at<Vec3b>(readPoint);
        }
    };

#pragma omp parallel for num_threads(4)
    for (int row = tlInfo.y; row < brInfo.y; ++row)
        processRow(row);

    infoImg.copyTo(io_img(infoRect));
}

void ImageUtils::showDebug([[maybe_unused]] const Mat &img, [[maybe_unused]] const std::string *pWindowName)
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
