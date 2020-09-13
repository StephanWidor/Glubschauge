#include "cv/GifCreate.h"
#include "FileSystem.h"

#include <thread>

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
        m_container.push(img);
        if (const auto diffTime = duration_cast<milliseconds>(system_clock::now() - m_startTime);
            diffTime >= m_duration)
        {
            std::thread(
              [diffTime](GifContainer container) {
                  if (FileSystem::requestPermission(FileSystem::AccessType::Write))
                  {
                      const auto path = FileSystem::generatePathForNewPicture("gif");
                      container.save(path, diffTime);
                  }
              },
              m_container)
              .detach();
            m_collecting = false;
            m_callbackAfterCollect();
        }
    }
}
