#pragma once
#include <chrono>
#include <filesystem>
#include <opencv2/core.hpp>
#include <vector>

namespace magick {

class GifContainer
{
public:
    static constexpr bool implemented()
    {
#ifdef IMAGEMAGICK_AVAILABLE
        return true;
#else
        return false;
#endif
    }

    void push(const cv::Mat &img)
    {
        assert(img.type() == CV_8UC3);
        m_images.push_back(img);
    }

    bool save(const std::filesystem::path &file, std::chrono::milliseconds duration);

    void clear() { m_images.clear(); }

private:
    std::vector<cv::Mat> m_images;
};

}    // namespace magick
