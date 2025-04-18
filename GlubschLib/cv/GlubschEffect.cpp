#include "cv/GlubschEffect.h"
#include "cv/FaceDistortion.h"
#include "cv/MouthOpen.h"
#include "cv/Utils2D.h"
#include "logger.h"

namespace cv {

GlubschConfig loadGlubschConfigFromYaml(const std::filesystem::path &path)
{
    GlubschConfig config;
    try
    {
        cv::FileStorage file(path.string(), cv::FileStorage::READ);
        std::vector<double> tmpDistortions;
        if (!file.isOpened())
            throw std::runtime_error("Failed to open File");
        file["drawLandmarks"] >> config.drawLandmarks;
        file["distortAlways"] >> config.distortAlways;
        file["distortions"] >> tmpDistortions;
        file.release();
        if (tmpDistortions.size() > config.distortions.size())
            throw std::runtime_error("Invalid distortions");
        std::copy(tmpDistortions.begin(), tmpDistortions.end(), config.distortions.begin());
    }
    catch (std::exception &e)
    {
        logger::out << std::format("Failed to load GlubschConfig from {}", path.string());
        logger::out << "Will use default configuration";
    }
    return config;
}

bool saveToYaml(const GlubschConfig &config, const std::filesystem::path &path)
{
    try
    {
        cv::FileStorage file(path.string(), cv::FileStorage::WRITE);
        if (file.isOpened())
        {
            file << "drawLandmarks" << config.drawLandmarks;
            file << "distortAlways" << config.distortAlways;
            file << "distortions" << std::vector(config.distortions.begin(), config.distortions.end());
            file.release();
            return true;
        }
    }
    catch (...)
    {
        logger::out << std::format("Failed to save GlubschConfig to {}", path.string());
    }
    return false;
}

void GlubschEffect::process(Mat &io_img)
{
    const auto doDistort =
      std::any_of(config.distortions.begin(), config.distortions.end(), [](const auto factor) { return factor > 0.0; });
    if (config.drawLandmarks || doDistort)
    {
        const auto [faceBBoxes, landmarks] = m_faceDetection.detect(io_img);
        if (config.drawLandmarks)
        {
            drawContours(io_img, toContours(faceBBoxes), -1, g_drawColor, 2);
            for (const auto &faceLandmarks : landmarks)
            {
                for (const auto &point : faceLandmarks)
                    circle(io_img, round(point), 1, g_drawColor, 2);
            }
        }
        if (doDistort)
        {
            const auto power = m_smoothing.push(config.distortAlways ? 1.0 : 4.0 * averageMouthOpenFactor(landmarks));
            if (power > 0.0)
                distort(io_img, createBarrelInfos(landmarks, power, config.distortions));
        }
    }
}

}    // namespace cv
