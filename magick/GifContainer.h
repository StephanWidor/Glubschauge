#pragma once

#include <opencv2/core.hpp>

#include <chrono>
#include <string>
#include <vector>

#ifdef ANDROID

#include "Logger.h"

namespace magick {

class GifContainer
{
public:
    void push(const cv::Mat &) { Logger::debug() << "magick::GifContainer::push not implemented"; }

    void save(const std::string &, std::chrono::milliseconds)
    {
        Logger::debug() << "magick::GifContainer::save not implemented";
    }
};

}    // namespace magick

#else

#if defined(__APPLE__)
#include <Magick++/Image.h>
#include <Magick++/STL.h>
#elif defined(__linux__)
#include <ImageMagick-6/Magick++/Image.h>
#include <ImageMagick-6/Magick++/STL.h>
#endif
namespace magick {

class GifContainer
{
public:
    void push(const cv::Mat &img);

    void save(const std::string &file, std::chrono::milliseconds duration);

private:
    std::vector<Magick::Image> m_images;
};

}    // namespace magick

#endif
