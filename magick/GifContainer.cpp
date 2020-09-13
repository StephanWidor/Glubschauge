#include "magick/GifContainer.h"
#include "FileSystem.h"

#include <thread>

void magick::GifContainer::push(const cv::Mat &img)
{
    assert(img.type() == CV_8UC3);
    m_images.push_back(Magick::Image(img.cols, img.rows, "BGR", Magick::CharPixel, img.data));
}

void magick::GifContainer::save(const std::string &file, std::chrono::milliseconds duration)
{
    if (!m_images.empty())
    {
        const size_t delay = duration.count() / (10u * m_images.size());
        for (auto &image : m_images)
            image.animationDelay(delay);
        Magick::writeImages(m_images.begin(), m_images.end(), file);
        m_images.clear();
    }
}
