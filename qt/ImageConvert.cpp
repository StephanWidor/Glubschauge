#include "qt/ImageConvert.h"
#include <Logger.h>
#include <opencv2/opencv.hpp>

namespace qt {

cv::Mat ImageConvert::toMat(QVideoFrame &frame)
{
    cv::Mat img(frame.height(), frame.width(), CV_8UC3);
    if (frame.map(QAbstractVideoBuffer::ReadOnly) && frame.isReadable())
    {
        QVideoFrame::PixelFormat frameFormat = frame.pixelFormat();
        switch (frameFormat)
        {
            case QVideoFrame::Format_YUYV:
            {
                cv::Mat yuvMat(frame.height(), frame.width(), CV_8UC2, frame.bits());
                cv::cvtColor(yuvMat, img, cv::COLOR_YUV2BGR_YUY2);
                break;
            }
            case QVideoFrame::Format_YUV420P:
            {
                cv::Mat yuv420pMat(frame.height() + frame.height() / 2, frame.width(), CV_8UC1, frame.bits());
                // Don't know why, but we need YUV420p2RGB, not YUV420p2BGR here!
                cv::cvtColor(yuv420pMat, img, cv::COLOR_YUV420p2RGB);
                break;
            }
            case QVideoFrame::Format_BGR32:
            {
                cv::Mat bgr32Mat(frame.height(), frame.width(), CV_8UC4, frame.bits());
                cv::cvtColor(bgr32Mat, img, cv::COLOR_BGRA2BGR);
                break;
            }
            case QVideoFrame::Format_RGB32:
            {
                cv::Mat rgb32Mat(frame.height(), frame.width(), CV_8UC4, frame.bits());
                cv::cvtColor(rgb32Mat, img, cv::COLOR_RGBA2RGB);
                break;
            }
            case QVideoFrame::Format_ABGR32:    // seems this is rather RGBA!
            {
                cv::Mat rgbaMat(frame.height(), frame.width(), CV_8UC4, frame.bits());
                cv::cvtColor(rgbaMat, img, cv::COLOR_RGBA2BGR);
                break;
            }
            case QVideoFrame::Format_ARGB32:    // seems this is rather BGRA!
            {
                cv::Mat bgraMat(frame.height(), frame.width(), CV_8UC4, frame.bits());
                cv::cvtColor(bgraMat, img, cv::COLOR_BGRA2BGR);
                break;
            }
            default:
                logger::out << "Couldn't convert " << frameFormat << "to cv::Mat";
                break;
        }
        frame.unmap();
    }
    else
        logger::out << "Couldn't map or read QVideoFrame";
    return img;
}

QVideoFrame ImageConvert::toQVideoFrame(const cv::Mat &img)
{
    assert(img.channels() == 3);
    cv::Mat bufferMat(img.size(), CV_8UC4);
    cv::cvtColor(img, bufferMat, cv::COLOR_RGB2BGRA);
    size_t numBytes = bufferMat.elemSize() * bufferMat.total();
    size_t bytesPerLine = bufferMat.elemSize() * bufferMat.cols;
    QVideoFrame frame(numBytes, QSize(img.cols, img.rows), bytesPerLine, QVideoFrame::Format_BGR32);
    if (frame.map(QAbstractVideoBuffer::WriteOnly) && frame.isWritable())
    {
        std::memcpy((void *)frame.bits(), (const void *)bufferMat.data, numBytes);
        frame.unmap();
    }
    else
        logger::out << "couldn't map frame";
    return frame;
}

}    // namespace qt
