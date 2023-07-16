#pragma once
#include "NumberSmoothing.h"
#include "cv/FaceDetection.h"
#include "cv/FaceDistortionType.h"

namespace cv {

class GlubschEffect
{
public:
    GlubschEffect(const std::string &cascadeDataPath, const std::string &facemarkDataPath)
        : m_faceDetection(cascadeDataPath, facemarkDataPath)
    {}

    void process(Mat &io_img);

    void setDrawLandmarks(bool draw) { m_drawLandmarks = draw; }

    bool getDrawLandmarks() const { return m_drawLandmarks; }

    void setDistortAlways(bool distortAlways) { m_distortAlways = distortAlways; }

    bool getDistortAlways() const { return m_distortAlways; }

    void setDistort(const FaceDistortionType type, double factor) { set(m_distortions, type, factor); }

    void increment(FaceDistortionType type)
    {
        const auto newValue = (std::floor(get(m_distortions, type) / incrementStep) + 1.0) * incrementStep;
        set(m_distortions, type, newValue);
    }

    void decrement(FaceDistortionType type)
    {
        const auto newValue = (std::floor(get(m_distortions, type) / incrementStep) - 1.0) * incrementStep;
        set(m_distortions, type, newValue);
    }

    double getDistort(const FaceDistortionType type) const { return get(m_distortions, type); }

private:
    FaceDetection m_faceDetection;
    NumberSmoothing m_smoothing{10u};
    static const inline Scalar g_drawColor = Scalar(255, 200, 0);
    bool m_drawLandmarks = false;
    bool m_distortAlways = false;
    FaceDistortions m_distortions = defaultFaceDistortions;
    static constexpr double incrementStep = 0.1;
};

}    // namespace cv
