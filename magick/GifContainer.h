#pragma once
#include <opencv2/core.hpp>
#ifdef IMAGEMAGICK_AVAILABLE
#include <Magick++/Image.h>
#include <Magick++/STL.h>
#endif

#include <chrono>
#include <string>
#include <vector>

namespace magick {

class GifContainer
{
public:
    void push(const cv::Mat &);

    bool save(const std::string &file, std::chrono::milliseconds duration);

    void clear();

private:
#ifdef IMAGEMAGICK_AVAILABLE
    std::vector<Magick::Image> m_images;    // swdebug: maybe store cv::Mat?
#endif
};

}    // namespace magick
