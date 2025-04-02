#pragma once
#include "cv/FaceDetection.h"
#include <numbers>

namespace cv {

class MouthOpen
{
public:
    static double calc(const FaceDetection::Landmarks &faceLandmarks)
    {
        const auto lipContour = FaceDetection::filter(faceLandmarks, FaceDetection::LandmarksType::InnerLip);
        const auto lipRect = cv::boundingRect(lipContour);
        const auto lipRange = static_cast<double>(std::max(lipRect.width, lipRect.height));
        const auto fullArea = lipRange * lipRange;
        const auto lipArea = cv::contourArea(lipContour);
        static constexpr double pi = 3.1415926535897931;    // 4.0*std::atan(1.0);
        return 1.0 - std::cos(std::numbers::pi * lipArea / fullArea);
    }

    static double calcAverage(const std::vector<FaceDetection::Landmarks> &landmarks)
    {
        if (landmarks.empty())
            return 0.0;
        std::vector<double> values;
        for (const auto &faceLandmarks : landmarks)
            values.push_back(calc(faceLandmarks));
        return std::accumulate(values.begin(), values.end(), 0.0) / static_cast<double>(values.size());
    }
};

}    // namespace cv
