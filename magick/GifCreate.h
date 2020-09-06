#pragma once

#if defined(ANDROID)
namespace Magick {
class Image
{};
}    // namespace Magick
#elif defined(__APPLE__)
#include <Magick++/Image.h>
#include <Magick++/STL.h>
#elif defined(__linux__)
#include <ImageMagick-6/Magick++/Image.h>
#include <ImageMagick-6/Magick++/STL.h>
#endif
#include <opencv2/core.hpp>

#include <chrono>
#include <string>
#include <vector>

namespace magick {

class GifCreate
{
public:
    void start(std::chrono::milliseconds duration, std::function<void()> callbackAfterCollect);

    void push(const cv::Mat &img);

    bool collecting() const { return m_collecting; }

    static constexpr bool isFunctional()
    {
#ifdef ANDROID
        return false;
#else
        return true;
#endif
    }

private:
    void save(const std::string &file, std::chrono::milliseconds duration);

    bool m_collecting = false;
    std::chrono::time_point<std::chrono::system_clock> m_startTime;
    std::chrono::milliseconds m_duration;
    std::function<void()> m_callbackAfterCollect;
    std::vector<Magick::Image> m_images;
};

}    // namespace magick
