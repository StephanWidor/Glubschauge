#include "magick/GifContainer.h"

#ifdef IMAGEMAGICK_AVAILABLE
#include <Magick++/Image.h>
#include <Magick++/STL.h>

bool magick::GifContainer::implemented()
{
    return true;
}

void magick::GifContainer::push(const cv::Mat &img)
{
    assert(img.type() == CV_8UC3);
    m_images.push_back(img);
}

bool magick::GifContainer::save(const std::string &file, std::chrono::milliseconds duration)
{
    bool retVal = false;
    if (!m_images.empty())
    {
        std::vector<Magick::Image> images;
        images.reserve(m_images.size());
        std::transform(m_images.begin(), m_images.end(), std::back_inserter(images), [](const auto &img) {
            return Magick::Image(img.cols, img.rows, "BGR", Magick::CharPixel, img.data);
        });

        const size_t delay = duration.count() / (10u * images.size());
        for (auto &image : images)
            image.animationDelay(delay);
        try
        {
            Magick::writeImages(images.begin(), images.end(), file);
            retVal = true;
        }
        catch (...)
        {}
        m_images.clear();
    }
    return retVal;
}

void magick::GifContainer::clear()
{
    m_images.clear();
}

#else    // IMAGEMAGICK_AVAILABLE

bool magick::GifContainer::implemented()
{
    return false;
}

void magick::GifContainer::push(const cv::Mat &)
{}

bool magick::GifContainer::save(const std::string &, std::chrono::milliseconds)
{
    return false;
}

void magick::GifContainer::clear()
{}

#endif    // IMAGEMAGICK_AVAILABLE
