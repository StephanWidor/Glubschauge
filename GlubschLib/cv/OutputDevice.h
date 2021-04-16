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

class OutputDevice
{
public:
    OutputDevice(std::string_view device);

    ~OutputDevice();

    void push(const cv::Mat &img);

    static constexpr bool implemented()
    {
#ifdef V4L_AVAILABLE
        return true;
#else
        return false;
#endif
    }

private:
    bool setSize(const cv::Size &size);

    cv::Size m_size{-1, -1};
    int m_output{-1};
};

}    // namespace cv
