#include "cv/BarrelCreation.h"

#include <numeric>

namespace cv {

std::vector<cv::ImageUtils::BarrelInfo>
  BarrelCreation::createBarrelInfos(const std::vector<FaceDetection::Landmarks> &landmarks, double power,
                                    const FaceDistortions &distortions)
{
    std::vector<cv::ImageUtils::BarrelInfo> barrelInfos;
    using LandmarksType = FaceDetection::LandmarksType;
    for (const auto &faceLandmarks : landmarks)
    {
        if (const auto factor = get(distortions, FaceDistortionType::Eyes); factor > 0.0)
        {
            const auto scaledPower = factor * power;
            barrelInfos.push_back(
              createBarrelInfo(FaceDetection::filter(faceLandmarks, LandmarksType::LeftEye), scaledPower));
            barrelInfos.push_back(
              createBarrelInfo(FaceDetection::filter(faceLandmarks, LandmarksType::RightEye), scaledPower));
        }
        if (const auto factor = get(distortions, FaceDistortionType::Nose); factor > 0.0)
        {
            barrelInfos.push_back(createBarrelInfo(
              FaceDetection::filter(faceLandmarks, LandmarksType::NoseBridge, LandmarksType::LowerNose),
              factor * power));
        }
        if (const auto factor = get(distortions, FaceDistortionType::Mouth); factor > 0.0)
        {
            barrelInfos.push_back(
              createBarrelInfo(FaceDetection::filter(faceLandmarks, LandmarksType::InnerLip), factor * power));
        }
        if (const auto factor = get(distortions, FaceDistortionType::UpperHead); factor > 0.0)
        {
            barrelInfos.push_back(createBarrelInfo(
              FaceDetection::filter(faceLandmarks, LandmarksType::LeftEyebrow, LandmarksType::RightEyebrow),
              factor * power));
        }
        if (const auto factor = get(distortions, FaceDistortionType::LowerHead); factor > 0.0)
        {
            const auto jaw = FaceDetection::filter(faceLandmarks, LandmarksType::JawLine);
            barrelInfos.push_back(createBarrelInfo({jaw.begin() + 4u, jaw.end() - 4u}, factor * power));
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
