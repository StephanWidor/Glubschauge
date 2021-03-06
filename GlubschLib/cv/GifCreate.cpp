#include "cv/GifCreate.h"
#include "logger.h"
#include <thread>

cv::GifCreate::~GifCreate()
{
    // busy wait until gif saving has finished
    while (processing())
        ;
}

bool cv::GifCreate::implemented()
{
    return magick::GifContainer::implemented();
}

void cv::GifCreate::start(const std::string &path, std::chrono::milliseconds duration,
                          std::function<void()> callbackAfterCollect, std::function<void()> callbackAfterProcessing)
{
    if (!m_collecting)
    {
        m_path = path;
        m_duration = duration;
        m_startTime = std::chrono::system_clock::now();
        m_callbackAfterCollect = callbackAfterCollect;
        m_callbackAfterProcessing = callbackAfterProcessing;
        m_collecting = true;
    }
}

void cv::GifCreate::push(const cv::Mat &img)
{
    using namespace std::chrono;
    if (m_collecting)
    {
        m_container.push(img);
        if (const auto diffTime = duration_cast<milliseconds>(system_clock::now() - m_startTime);
            diffTime >= m_duration)
        {
            ++m_processing;
            std::thread(
              [diffTime, this, path = m_path](magick::GifContainer container) {
                  if (!container.save(path, diffTime))
                      logger::out << "failed to save " << path;
                  --m_processing;
                  m_callbackAfterProcessing();
              },
              m_container)
              .detach();
            m_container.clear();
            m_collecting = false;
            m_callbackAfterCollect();
        }
    }
}
