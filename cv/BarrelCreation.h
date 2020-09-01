#pragma once
#include "cv/FaceDetection.h"
#include "cv/FaceDistortionType.h"
#include "cv/ImageUtils.h"
#include "cv/Utils2D.h"

#include <chrono>
#include <deque>

namespace cv {

class BarrelCreation
{
public:
    static std::vector<cv::ImageUtils::BarrelInfo>
      createBarrelInfos(const std::vector<FaceDetection::Landmarks> &landmarks, double power,
                        unsigned distortionTypes = static_cast<unsigned>(FaceDistortionType::Eyes));

    static ImageUtils::BarrelInfo createBarrelInfo(const FaceDetection::Landmarks &landmarks, double power);
};

}    // namespace cv
