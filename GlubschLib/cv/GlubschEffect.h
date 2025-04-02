#pragma once
#include "NumberSmoothing.h"
#include "cv/FaceDetection.h"
#include "cv/FaceDistortionType.h"
#include <filesystem>

namespace cv {

struct GlubschConfig
{
    bool drawLandmarks = false;
    bool distortAlways = false;
    FaceDistortions distortions = defaultFaceDistortions;
};
GlubschConfig loadGlubschConfigFromYaml(const std::filesystem::path &);
bool saveToYaml(const GlubschConfig &, const std::filesystem::path &);

class GlubschEffect
{
public:
    GlubschEffect(const std::string &cascadeDataPath, const std::string &facemarkDataPath,
                  GlubschConfig initConfig = GlubschConfig{})
        : config{initConfig}, m_faceDetection(cascadeDataPath, facemarkDataPath)
    {}

    void process(Mat &io_img);

    GlubschConfig config;

private:
    FaceDetection m_faceDetection;
    NumberSmoothing m_smoothing{10u};
    static const inline Scalar g_drawColor = Scalar(255, 200, 0);
};

}    // namespace cv
