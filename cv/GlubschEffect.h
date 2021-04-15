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

    void setDoDistort(FaceDistortionType type, bool distort)
    {
        if (distort)
            m_distortionTypes |= type;
        else
            m_distortionTypes &= ~type;
    }

    bool getDoDistort() const { return m_distortionTypes != FaceDistortionType::None; }

    bool getDoDistort(FaceDistortionType type) const { return m_distortionTypes & type; }

private:
    FaceDetection m_faceDetection;
    NumberSmoothing m_smoothing{10u};
    static const inline Scalar g_drawColor = Scalar(255, 200, 0);
    bool m_drawLandmarks = false;
    bool m_distortAlways = false;
    unsigned m_distortionTypes = FaceDistortionType::Eyes | FaceDistortionType::Mouth;
};

}    // namespace cv
