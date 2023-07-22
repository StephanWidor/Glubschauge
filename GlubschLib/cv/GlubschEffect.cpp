#include "cv/GlubschEffect.h"
#include "cv/BarrelCreation.h"
#include "cv/MouthOpen.h"

namespace cv {

void GlubschEffect::process(Mat &io_img)
{
    const auto doDistort = !std::all_of(config.distortions.begin(), config.distortions.end(),
                                        [](const auto factor) { return factor > 0.0; });
    if (config.drawLandmarks || doDistort)
    {
        const auto [faceBBoxes, landmarks] = m_faceDetection.detect(io_img);
        if (config.drawLandmarks)
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
            const auto power = m_smoothing.push(config.distortAlways ? 1.0 : 4.0 * MouthOpen::calcAverage(landmarks));
            if (power > 0.0)
                ImageUtils::distort(io_img, BarrelCreation::createBarrelInfos(landmarks, power, config.distortions));
        }
    }
}

}    // namespace cv
