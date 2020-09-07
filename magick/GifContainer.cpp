#include "magick/GifContainer.h"
#include "FileSystem.h"

#include <thread>

#ifndef ANDROID

void magick::GifContainer::push(const cv::Mat &img)
{
    m_images.push_back(Magick::Image(img.cols, img.rows, "BGR", Magick::CharPixel, img.data));
}

void magick::GifContainer::save(const std::string &file, std::chrono::milliseconds duration)
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
