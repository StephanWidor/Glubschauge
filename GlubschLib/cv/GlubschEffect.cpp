#include "cv/GlubschEffect.h"
#include "cv/BarrelCreation.h"
#include "cv/MouthOpen.h"

namespace cv {

void GlubschEffect::process(Mat &io_img)
{
    const auto doDistort =
      !std::all_of(m_distortions.begin(), m_distortions.end(), [](const auto factor) { return factor > 0.0; });
    if (m_drawLandmarks || doDistort)
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
        if (doDistort)
        {
            const auto power = m_smoothing.push(m_distortAlways ? 1.0 : 4.0 * MouthOpen::calcAverage(landmarks));
            if (power > 0.0)
                ImageUtils::distort(io_img, BarrelCreation::createBarrelInfos(landmarks, power, m_distortions));
        }
    }
}

}    // namespace cv
