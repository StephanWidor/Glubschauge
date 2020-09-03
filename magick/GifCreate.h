#pragma once
#if defined(ANDROID)
// let's see...
#elif defined(__APPLE__)
#include <Magick++/Image.h>
#include <Magick++/STL.h>
#elif defined(__linux__)
#include <ImageMagick-6/Magick++/Image.h>
#include <ImageMagick-6/Magick++/STL.h>
#endif
#include <opencv2/core.hpp>

#include <string>
#include <vector>

namespace magick {

class GifCreate
{
public:
    void push(const cv::Mat &img);

    void save(const std::string &file, bool inExtraThread = true);

    size_t size() const { return m_images.size(); }

private:
    std::vector<Magick::Image> m_images;
};

}    // namespace magick
