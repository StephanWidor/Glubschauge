#include "cv/SaveUtils.h"
#include <stddef.h>

#ifdef IMAGEMAGICK_AVAILABLE

#include <Magick++/Image.h>
#include <Magick++/STL.h>
#include <ranges>

bool cv::saveGif(const std::span<TimeStampedImg> images, const std::filesystem::path &filePath)
{
    if (images.empty())
        return false;

    auto magickImages = images | std::views::keys | std::views::transform([](auto &cvImg) {
                            auto img = Magick::Image(cvImg.cols, cvImg.rows, "BGR", Magick::CharPixel, cvImg.data);
                            return img;
                        }) |
                        std::ranges::to<std::vector>();
    const auto delayDiff = [](const auto t0, const auto t1) {
        const auto diffMilliSeconds = duration_cast<std::chrono::milliseconds>(t1 - t0).count();
        return static_cast<size_t>(std::round(0.1 * static_cast<double>(diffMilliSeconds)));
    };
    for (auto i = 1u; i < magickImages.size(); ++i)
        magickImages[i - 1].animationDelay(delayDiff(images[i - 1].second, images[i].second));
    magickImages.back().animationDelay(0u);

    try
    {
        Magick::writeImages(magickImages.begin(), magickImages.end(), filePath.native());
        return true;
    }
    catch (...)
    {
        return false;
    }
}

#else    // IMAGEMAGICK_AVAILABLE

bool cv::saveGif(const std::span<TimeStampedImg>, const std::filesystem::path &)
{
    return false;
}

#endif    // IMAGEMAGICK_AVAILABLE
