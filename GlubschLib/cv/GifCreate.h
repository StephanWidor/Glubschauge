#pragma once
#include "magick/GifContainer.h"
#include <atomic>
#include <chrono>
#include <string>
#include <vector>

namespace cv {

class GifCreate
{
public:
    ~GifCreate();

    static bool implemented();

    void start(const std::string &path, std::chrono::milliseconds duration, std::function<void()> callbackAfterCollect,
               std::function<void()> callbackAfterProcessing);

    void push(const cv::Mat &img);

    bool collecting() const { return m_collecting; }

    bool processing() const { return m_processing > 0; }

private:
    void save(const std::string &file, std::chrono::milliseconds duration);

    bool m_collecting = false;
    std::atomic<size_t> m_processing{0};
    std::chrono::time_point<std::chrono::system_clock> m_startTime;
    std::string m_path;
    std::chrono::milliseconds m_duration;
    std::function<void()> m_callbackAfterCollect;
    std::function<void()> m_callbackAfterProcessing;
    magick::GifContainer m_container;
};

}    // namespace cv
