#include "cv/GlubschEffect.h"
#include "cv/BarrelCreation.h"
#include "cv/MouthOpen.h"

namespace cv {

void GlubschEffect::process(Mat &io_img)
{
    if (m_drawLandmarks || getDoDistort())
    {
        const auto [faceBBoxes, landmarks] = m_faceDetection.detect(io_img);
        if (m_drawLandmarks)
        {
            drawContours(io_img, Utils2D::toContours(faceBBoxes), -1, g_drawColor, 2);
            for (const auto &faceLandmarks : landmarks)
            {
                for (const auto &point : faceLandmarks)
                    circle(io_img, Utils2D::round(point), 1, g_drawColor, 2);
            }
        }
        if (getDoDistort())
        {
            const auto power = m_smoothing.push(m_distortAlways ? 1.0 : 4.0 * MouthOpen::calcAverage(landmarks));
            if (power > 0.0)
                ImageUtils::distort(io_img, BarrelCreation::createBarrelInfos(landmarks, power, m_distortionTypes));
        }
    }
}

}    // namespace cv
