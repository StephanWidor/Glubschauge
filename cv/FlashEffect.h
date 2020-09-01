#pragma once
#include <opencv2/opencv.hpp>

#include <chrono>

namespace cv {

class FlashEffect
{
public:
    void trigger();

    void process(Mat &io_img);

    bool flashing() const { return m_flashing; }

private:
    std::chrono::time_point<std::chrono::system_clock> m_startTime;
    static constexpr double m_duration = 300.0;
    bool m_flashing = false;
};

}    // namespace cv
