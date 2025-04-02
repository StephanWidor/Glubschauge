#pragma once
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>

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

static constexpr double defaultIncrementStep = 0.1;

inline void increment(FaceDistortions &distortions, FaceDistortionType type,
                      double incrementStep = defaultIncrementStep)
{
    const auto newValue = (std::floor(get(distortions, type) / incrementStep) + 1.0) * incrementStep;
    set(distortions, type, newValue);
}

inline void decrement(FaceDistortions &distortions, FaceDistortionType type,
                      double incrementStep = defaultIncrementStep)
{
    const auto newValue = (std::floor(get(distortions, type) / incrementStep) - 1.0) * incrementStep;
    set(distortions, type, newValue);
}

}    // namespace cv
