#include "cv/GifCreate.h"
#include "FileSystem.h"

#include <opencv2/imgproc/imgproc.hpp>

void cv::GifCreate::start(std::chrono::milliseconds duration, std::function<void()> callbackAfterCollect)
{
    if (!m_collecting)
    {
        m_duration = duration;
        m_startTime = std::chrono::system_clock::now();
        m_callbackAfterCollect = callbackAfterCollect;
        m_collecting = true;
    }
}

void cv::GifCreate::push(const cv::Mat &img)
{
    using namespace std::chrono;
    if (m_collecting)
    {
        assert(img.type() == CV_8UC3);
        m_container.push(img);
        if (const auto diffTime = duration_cast<milliseconds>(system_clock::now() - m_startTime);
            diffTime >= m_duration)
        {
            m_container.save(FileSystem::generatePathForNewPicture("gif"), diffTime);
            m_collecting = false;
            m_callbackAfterCollect();
        }
    }
}
