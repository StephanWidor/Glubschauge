#ifndef ANDROID

#include "magick/GifCreate.h"

#include <opencv2/imgproc/imgproc.hpp>

#include <thread>

void magick::GifCreate::push(const cv::Mat &img)
{
    assert(img.type() == CV_8UC3);
    cv::Mat colorConvertedImg;
    cv::cvtColor(img, colorConvertedImg, cv::COLOR_BGR2BGRA);
    m_images.push_back(Magick::Image(img.cols, img.rows, "BGRA", Magick::CharPixel, (char *)colorConvertedImg.data));
}

void magick::GifCreate::save(const std::string &file, bool inExtraThread)
{
    if (!m_images.empty())
    {
        if (inExtraThread)
        {
            std::thread([](std::vector<Magick::Image> images,
                           std::string file) { Magick::writeImages(images.begin(), images.end(), file); },
                        m_images, file)
              .detach();
        }
        else
            Magick::writeImages(m_images.begin(), m_images.end(), file);
        m_images.clear();
    }
}

#endif    // ANDROID
