#include "cv/V4lLoopbackDevice.h"
#include "logger.h"
#include <opencv2/opencv.hpp>

bool cv::V4lLoopbackDevice::setSize([[maybe_unused]] const cv::Size &size)
{
#ifdef V4L_AVAILABLE
    if (size == m_size)
        return true;
    if (m_output < 0)
        return false;

    v4l2_format format{.type = V4L2_BUF_TYPE_VIDEO_OUTPUT};
    if (ioctl(m_output, VIDIOC_G_FMT, &format) < 0)
        return false;

    format.fmt.pix.width = size.width;
    format.fmt.pix.height = size.height;
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;
    //format.fmt.pix.sizeimage = size.width * size.height * 3;
    format.fmt.pix.sizeimage = 3 * size.width * size.height / 2;
    format.fmt.pix.field = V4L2_FIELD_NONE;

    if (ioctl(m_output, VIDIOC_S_FMT, &format) < 0)
        return false;

    m_size = size;
    return true;
#else
    return false;
#endif
}

cv::V4lLoopbackDevice::V4lLoopbackDevice([[maybe_unused]] std::string_view device)
#ifdef V4L_AVAILABLE
    : m_output(::open(device.data(), O_WRONLY))
#endif
{}

cv::V4lLoopbackDevice::~V4lLoopbackDevice()
{
#ifdef V4L_AVAILABLE
    if (m_output >= 0)
        close(m_output);
#endif
}

void cv::V4lLoopbackDevice::push([[maybe_unused]] const cv::Mat &img)
{
#ifdef V4L_AVAILABLE
    if (!setSize(img.size()))
        return;
    cv::Mat yuv;
    cv::cvtColor(img, yuv, cv::COLOR_BGR2YUV_I420);
    if (::write(m_output, yuv.data, yuv.cols * yuv.rows) < 0)
        logger::out << "ERROR: could not write to output device!\n";
#endif
}
