#include "magick/GifCreate.h"
#include "FileSystem.h"
#include "Logger.h"

#include <opencv2/imgproc/imgproc.hpp>

#include <thread>

#if defined(ANDROID)

void magick::GifCreate::start(std::chrono::milliseconds, std::function<void()>)
{}
void magick::GifCreate::push(const cv::Mat &)
{}

#else

void magick::GifCreate::start(std::chrono::milliseconds duration, std::function<void()> callbackAfterCollect)
{
    if (!m_collecting)
    {
        m_duration = duration;
        m_startTime = std::chrono::system_clock::now();
        m_callbackAfterCollect = callbackAfterCollect;
        m_collecting = true;
    }
}

void magick::GifCreate::push(const cv::Mat &img)
{
    using namespace std::chrono;
    if (m_collecting)
    {
        assert(img.type() == CV_8UC3);
        m_images.push_back(Magick::Image(img.cols, img.rows, "BGR", Magick::CharPixel, img.data));
        if (const auto diffTime = duration_cast<milliseconds>(system_clock::now() - m_startTime);
            diffTime >= m_duration)
        {
            save(FileSystem::generatePathForNewPicture("gif"), diffTime);
            m_collecting = false;
            m_callbackAfterCollect();
        }
    }
}

void magick::GifCreate::save(const std::string &file, std::chrono::milliseconds duration)
{
    if (!m_images.empty())
    {
        const size_t delay = duration.count() / (10u * m_images.size());
        for (auto &image : m_images)
            image.animationDelay(delay);
        std::thread([](std::vector<Magick::Image> images,
                       std::string file) { Magick::writeImages(images.begin(), images.end(), file); },
                    m_images, file)
          .detach();
        m_images.clear();
    }
}

#endif    // ANDROID
