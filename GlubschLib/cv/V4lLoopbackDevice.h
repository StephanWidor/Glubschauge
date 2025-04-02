#pragma once
#if !defined(ANDROID) && __has_include(<sys/ioctl.h>)
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <unistd.h>
#define V4L_AVAILABLE
#endif

#include <opencv2/core.hpp>
#include <string_view>

namespace cv {

class V4lLoopbackDevice
{
public:
    V4lLoopbackDevice(std::string_view device);

    ~V4lLoopbackDevice();

    void push(const cv::Mat &img);

    static constexpr bool implemented()
    {
#ifdef V4L_AVAILABLE
        return true;
#else
        return false;
#endif
    }

    bool ok() const { return m_output >= 0; }

private:
    bool setSize(const cv::Size &size);

    cv::Size m_size{-1, -1};
    int m_output{-1};
};

}    // namespace cv
