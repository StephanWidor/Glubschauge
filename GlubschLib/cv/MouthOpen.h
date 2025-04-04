#pragma once
#include "cv/FaceDetection.h"
#include <algorithm>
#include <numbers>
#include <numeric>

namespace cv {

inline double mouthOpenFactor(const FaceDetection::Landmarks &faceLandmarks)
{
    const auto lipContour = FaceDetection::filter(faceLandmarks, FaceDetection::LandmarksType::InnerLip);
    const auto lipRect = cv::boundingRect(lipContour);
    const auto lipRange = static_cast<double>(std::max(lipRect.width, lipRect.height));
    const auto fullArea = lipRange * lipRange;
    const auto lipArea = cv::contourArea(lipContour);
    static constexpr double pi = 3.1415926535897931;    // 4.0*std::atan(1.0);
    return 1.0 - std::cos(std::numbers::pi * lipArea / fullArea);
}

inline double averageMouthOpenFactor(const std::vector<FaceDetection::Landmarks> &landmarks)
{
    if (landmarks.empty())
        return 0.0;

    return std::accumulate(
             landmarks.begin(), landmarks.end(), 0.0,
             [](auto accumulator, const auto &landmark) { return accumulator + mouthOpenFactor(landmark); }) /
           static_cast<double>(landmarks.size());
}

}    // namespace cv
