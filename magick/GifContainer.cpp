#include "magick/GifContainer.h"
#include "FileSystem.h"

#ifdef IMAGEMAGICK_AVAILABLE

void magick::GifContainer::push(const cv::Mat &img)
{
    assert(img.type() == CV_8UC3);
    m_images.push_back(Magick::Image(img.cols, img.rows, "BGR", Magick::CharPixel, img.data));
}

bool magick::GifContainer::save(const std::string &file, std::chrono::milliseconds duration)
{
    bool retVal = false;
    if (!m_images.empty())
    {
        const size_t delay = duration.count() / (10u * m_images.size());
        for (auto &image : m_images)
            image.animationDelay(delay);
        try
        {
            Magick::writeImages(m_images.begin(), m_images.end(), file);
            retVal = true;
        }
        catch (...)
        {}
    }
    m_images.clear();
    return retVal;
}

void magick::GifContainer::clear()
{
    m_images.clear();
}

#else    // IMAGEMAGICK_AVAILABLE

void magick::GifContainer::push(const cv::Mat &)
{}

bool magick::GifContainer::save(const std::string &, std::chrono::milliseconds)
{
    return false;
}

void magick::GifContainer::clear()
{}

#endif    // IMAGEMAGICK_AVAILABLE
