#pragma once
#include "cv/FaceDetection.h"
#include "cv/FaceDistortionType.h"
#include "cv/ImageUtils.h"

namespace cv {

class BarrelCreation
{
public:
    static std::vector<cv::ImageUtils::BarrelInfo>
      createBarrelInfos(const std::vector<FaceDetection::Landmarks> &landmarks, double power,
                        const FaceDistortions &distortions);

    static ImageUtils::BarrelInfo createBarrelInfo(const FaceDetection::Landmarks &landmarks, double power);
};

}    // namespace cv
