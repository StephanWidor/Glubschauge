#pragma once
#include <chrono>
#include <filesystem>
#include <opencv2/core.hpp>
#include <span>

namespace cv {

using TimeStamp = std::chrono::time_point<std::chrono::system_clock>;
using TimeStampedImg = std::pair<Mat, TimeStamp>;

static constexpr bool gifSaveAvailable()
{
#ifdef IMAGEMAGICK_AVAILABLE
    return true;
#else
    return false;
#endif
}

bool saveGif(std::span<TimeStampedImg>, const std::filesystem::path &);

}    // namespace cv