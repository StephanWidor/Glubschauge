#include "cv/FpsEffect.h"

namespace cv {

void FpsEffect::process(Mat &io_img)
{
    const auto fps = static_cast<int>(std::round(m_counter.tick()));
    if (fps > 0)
        drawText(io_img, String(std::to_string(fps) + " fps"));
}

void FpsEffect::drawText(Mat &io_img, const String &text)
{
    const int fontFace = FONT_HERSHEY_DUPLEX;
    const double fontScale = 1.0;
    const int thickness = 1;
    int baseline = 0;
    Size textSize = getTextSize(text, fontFace, fontScale, thickness, &baseline);
    Point textOrigin(0.5 * textSize.height, 1.5 * textSize.height);
    const auto &imgColor = io_img.at<Vec3b>(textOrigin);
    const Vec3b textColor(imgColor[0] > 127 ? 0 : 255, imgColor[1] > 127 ? 0 : 255, imgColor[2] > 127 ? 0 : 255);
    putText(io_img, text, textOrigin, fontFace, fontScale, textColor, thickness, 8);
}

}    // namespace cv
