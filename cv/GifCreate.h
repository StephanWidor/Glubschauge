#pragma once

#include "magick/GifContainer.h"
using GifContainer = magick::GifContainer;

#include <chrono>
#include <string>
#include <vector>

namespace cv {

class GifCreate
{
public:
    void start(std::chrono::milliseconds duration, std::function<void()> callbackAfterCollect);

    void push(const cv::Mat &img);

    bool collecting() const { return m_collecting; }

private:
    void save(const std::string &file, std::chrono::milliseconds duration);

    bool m_collecting = false;
    std::chrono::time_point<std::chrono::system_clock> m_startTime;
    std::chrono::milliseconds m_duration;
    std::function<void()> m_callbackAfterCollect;
    GifContainer m_container;
};

}    // namespace cv
