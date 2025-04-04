#include "cv/FaceDistortion.h"
#include "cv/Utils2D.h"

#include <numeric>
#include <omp.h>

namespace cv {

void distort(Mat &io_img, const BarrelInfo &barrelInfo)
{
    assert(io_img.type() == CV_8UC3);
    if (barrelInfo.radius <= 0.0 || barrelInfo.power <= 0.0)
        return;

    const auto infoRect = boundingRect(barrelInfo) & cv::Rect(0, 0, io_img.cols - 1, io_img.rows - 1);
    const auto tlInfo = infoRect.tl();
    const auto brInfo = infoRect.br();
    Mat infoImg = io_img(infoRect).clone();

    // OpenCV memory order is row first, so parallelizing row processing should be better than columns
    const auto processRow = [&](int row) {
        for (int col = tlInfo.x; col <= brInfo.x; ++col)
        {
            Point point(col, row);
            const auto diff = point - barrelInfo.center;
            double dist = norm(diff);
            if (dist == 0.0 || dist >= barrelInfo.radius)
                continue;
            double distRatio = std::pow(dist / barrelInfo.radius, barrelInfo.power);
            Point readPoint = barrelInfo.center + distRatio * diff;
            if (readPoint == point)
                continue;
            infoImg.at<Vec3b>(point - tlInfo) = io_img.at<Vec3b>(readPoint);
        }
    };

#pragma omp parallel for num_threads(4)
    for (int row = tlInfo.y; row < brInfo.y; ++row)
        processRow(row);

    infoImg.copyTo(io_img(infoRect));
}

std::vector<BarrelInfo> createBarrelInfos(const std::vector<FaceDetection::Landmarks> &landmarks, double power,
                                          const FaceDistortions &distortions)
{
    const auto createBarrelInfo = [](const FaceDetection::Landmarks &landmarks, double power) -> BarrelInfo {
        const auto baryCenter = round(std::accumulate(landmarks.begin(), landmarks.end(), Point2f(0.0f, 0.0f)) /
                                      static_cast<float>(landmarks.size()));
        cv::Rect boundingRect = cv::boundingRect(landmarks);
        double radius = cv::norm(boundingRect.tl() - boundingRect.br());
        return {baryCenter, radius, power};
    };

    std::vector<BarrelInfo> barrelInfos;
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

}    // namespace cv
