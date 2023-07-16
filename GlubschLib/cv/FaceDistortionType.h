#pragma once
#include <algorithm>
#include <array>
#include <cassert>
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

constexpr FaceDistortions defaultFaceDistortions{1.0, 0.0, 1.0, 0.0, 0.0};

}    // namespace cv
