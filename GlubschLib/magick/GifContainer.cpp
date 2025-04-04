#include "magick/GifContainer.h"
#include <logger.h>

#ifdef IMAGEMAGICK_AVAILABLE

#include <Magick++/Image.h>
#include <Magick++/STL.h>
#include <ranges>

bool magick::GifContainer::save(const std::filesystem::path &file, std::chrono::milliseconds duration)
{
    bool gifSaved = false;
    if (!m_images.empty())
    {
        const auto delay = static_cast<size_t>(
          std::round(0.1 * static_cast<double>(duration.count()) / static_cast<double>(m_images.size())));
        auto images = m_images | std::views::transform([delay](auto &cvImg) {
                          auto img = Magick::Image(cvImg.cols, cvImg.rows, "BGR", Magick::CharPixel, cvImg.data);
                          img.animationDelay(delay);
                          return img;
                      }) |
                      std::ranges::to<std::vector>();
        try
        {
            Magick::writeImages(images.begin(), images.end(), file.native());
            gifSaved = true;
        }
        catch (...)
        {}
        clear();
    }
    return gifSaved;
}

#else    // IMAGEMAGICK_AVAILABLE

bool magick::GifContainer::save(const std::filesystem::path &, std::chrono::milliseconds)
{
    return false;
}

#endif    // IMAGEMAGICK_AVAILABLE
