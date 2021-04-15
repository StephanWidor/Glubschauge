#include "cv/FlashEffect.h"

using namespace std::chrono;

void cv::FlashEffect::trigger()
{
    m_startTime = system_clock::now();
    m_flashing = true;
}

void cv::FlashEffect::process(Mat &io_img)
{
    if (m_flashing)
    {
        const auto diffTime =
          static_cast<double>(duration_cast<milliseconds>(system_clock::now() - m_startTime).count());
        if (diffTime > m_duration)
            m_flashing = false;
        else
        {
            static constexpr double twoPi = 6.2831853071795862;    // 8.0*std::atan(1.0);
            const auto toAdd = saturate_cast<uchar>(-127.5 * std::cos(diffTime * twoPi / m_duration) + 127.5);
            io_img += cv::Scalar(toAdd, toAdd, toAdd);
        }
    }
}
