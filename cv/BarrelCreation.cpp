#include "cv/BarrelCreation.h"

#include <numeric>

namespace cv {

std::vector<cv::ImageUtils::BarrelInfo>
  BarrelCreation::createBarrelInfos(const std::vector<FaceDetection::Landmarks> &landmarks, double power,
                                    unsigned distortionTypes)
{
    std::vector<cv::ImageUtils::BarrelInfo> barrelInfos;
    using LandmarksType = FaceDetection::LandmarksType;
    for (const auto &faceLandmarks : landmarks)
    {
        if (distortionTypes & FaceDistortionType::Eyes)
        {
            barrelInfos.push_back(
              createBarrelInfo(FaceDetection::filter(faceLandmarks, LandmarksType::LeftEye), power));
            barrelInfos.push_back(
              createBarrelInfo(FaceDetection::filter(faceLandmarks, LandmarksType::RightEye), power));
        }
        if (distortionTypes & FaceDistortionType::Nose)
            barrelInfos.push_back(createBarrelInfo(
              FaceDetection::filter(faceLandmarks, LandmarksType::NoseBridge, LandmarksType::LowerNose), power));
        if (distortionTypes & FaceDistortionType::Mouth)
            barrelInfos.push_back(
              createBarrelInfo(FaceDetection::filter(faceLandmarks, LandmarksType::InnerLip), power));
        if (distortionTypes & FaceDistortionType::UpperHead)
        {
            barrelInfos.push_back(createBarrelInfo(
              FaceDetection::filter(faceLandmarks, LandmarksType::LeftEyebrow, LandmarksType::RightEyebrow), power));
        }
        if (distortionTypes & FaceDistortionType::LowerHead)
        {
            const auto jaw = FaceDetection::filter(faceLandmarks, LandmarksType::JawLine);
            barrelInfos.push_back(createBarrelInfo({jaw.begin() + 4u, jaw.end() - 4u}, power));
        }
    }
    return barrelInfos;
}

ImageUtils::BarrelInfo BarrelCreation::createBarrelInfo(const FaceDetection::Landmarks &landmarks, double power)
{
    const auto baryCenter = Utils2D::round(std::accumulate(landmarks.begin(), landmarks.end(), Point2f(0.0f, 0.0f)) /
                                           static_cast<float>(landmarks.size()));
    cv::Rect boundingRect = cv::boundingRect(landmarks);
    double radius = cv::norm(boundingRect.tl() - boundingRect.br());
    return {baryCenter, radius, power};
}

}    // namespace cv
