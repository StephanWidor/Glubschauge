#pragma once
#include <chrono>
#include <opencv2/core.hpp>
#include <string>
#include <vector>

namespace magick {

class GifContainer
{
public:
    static bool implemented();

    void push(const cv::Mat &);

    bool save(const std::string &file, std::chrono::milliseconds duration);

    void clear();

private:
    std::vector<cv::Mat> m_images;
};

}    // namespace magick
