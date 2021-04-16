#pragma once
#include <QVideoFrame>
#include <opencv2/core.hpp>

namespace qt {

class ImageConvert
{
public:
    /// Convert QVideoFrame to cv::Mat
    static cv::Mat toMat(QVideoFrame &frame);

    /// Convert cv::Mat img to QVideoFrame in QVideoFrame::Format_BGR32
    static QVideoFrame toQVideoFrame(const cv::Mat &img);
};

}    // namespace qt
