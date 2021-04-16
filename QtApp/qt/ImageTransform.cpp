#include "qt/ImageTransform.h"
#include <QGuiApplication>

namespace qt {

void ImageTransform::transformFromCamera(cv::Mat &io_img) const
{
#if defined(ANDROID)
    cv::flip(io_img, io_img, 0);    // whyever this is neccessary... bug in qml VideoOutput?
    if (m_doCameraTransform)
        rotate(io_img, m_rotation);
#elif defined(_WIN32)
    if (m_doCameraTransform)
        cv::flip(io_img, io_img, -1);
#else
    if (m_doCameraTransform)
        cv::flip(io_img, io_img, 1);
#endif
}    // namespace qt

void ImageTransform::transformToCamera(cv::Mat &io_img) const
{
#if defined(ANDROID)
    if (m_doCameraTransform)
        rotate(io_img, -m_rotation);
#elif defined(_WIN32)
    if (m_doCameraTransform)
        cv::flip(io_img, io_img, 0);
#else
    (void)io_img;
#endif
}

void ImageTransform::rotate(cv::Mat &io_img, int rotation)
{
    if (std::abs(rotation) < 45)
        return;
    rotation = rotation % 360;
    if (rotation < 0)
        rotation += 360;
    if (rotation < 135)
        cv::rotate(io_img, io_img, cv::ROTATE_90_COUNTERCLOCKWISE);
    else if (rotation > 225)
        cv::rotate(io_img, io_img, cv::ROTATE_90_CLOCKWISE);
    else
        cv::rotate(io_img, io_img, cv::ROTATE_180);
}

}    // namespace qt
