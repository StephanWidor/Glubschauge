#pragma once
#include "cv/FaceDetection.h"

namespace cv {

enum class FaceDistortionType : size_t
{
    Eyes = 0,
    Nose,
    Mouth,
    UpperHead,
    LowerHead,
    Num
};

using FaceDistortions = std::array<double, static_cast<size_t>(FaceDistortionType::Num)>;
constexpr FaceDistortions defaultFaceDistortions{0.3, 0.9, 0.1, 0.1, 0.0};

constexpr double get(const FaceDistortions &distortions, const FaceDistortionType &type)
{
    assert(type < FaceDistortionType::Num);
    return distortions[static_cast<size_t>(type)];
}

constexpr void set(FaceDistortions &distortions, const FaceDistortionType &type, const double factor)
{
    assert(type < FaceDistortionType::Num);
    distortions[static_cast<size_t>(type)] = std::clamp(factor, 0.0, 1.0);
}

constexpr double defaultIncrementStep = 0.1;

inline void increment(FaceDistortions &distortions, FaceDistortionType type,
                      double incrementStep = defaultIncrementStep)
{
    set(distortions, type, (std::floor(get(distortions, type) / incrementStep) + 1.0) * incrementStep);
}

inline void decrement(FaceDistortions &distortions, FaceDistortionType type,
                      double incrementStep = defaultIncrementStep)
{
    set(distortions, type, (std::floor(get(distortions, type) / incrementStep) - 1.0) * incrementStep);
}

struct BarrelInfo
{
    Point center;
    double radius = 0.0;
    double power = 0.0;
};

inline Rect boundingRect(const BarrelInfo &info)
{
    const auto &center = info.center;
    const auto radius = static_cast<int>(std::ceil(info.radius));
    return cv::Rect(center.x - radius, center.y - radius, 2 * radius, 2 * radius);
}

void distort(Mat &io_img, const BarrelInfo &barrelInfo);

inline void distort(Mat &io_img, const std::vector<BarrelInfo> &barrelInfos)
{
    for (const auto &info : barrelInfos)
        distort(io_img, info);
}

std::vector<BarrelInfo> createBarrelInfos(const std::vector<FaceDetection::Landmarks> &landmarks, double power,
                                          const FaceDistortions &distortions);

}    // namespace cv
