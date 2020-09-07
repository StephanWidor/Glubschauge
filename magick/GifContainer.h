#pragma once
#include <Magick++/Image.h>
#include <Magick++/STL.h>
#include <opencv2/core.hpp>

#include <chrono>
#include <string>
#include <vector>

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
