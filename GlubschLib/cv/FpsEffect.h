#pragma once
#include "FpsCounter.h"

#include <opencv2/imgproc.hpp>

namespace cv {

class FpsEffect
{
public:
    void setEnabled(bool enabled)
    {
        if (enabled)
            m_counter.start();
        else
            m_counter.stop();
    }

    bool enabled() const { return m_counter.running(); }

    void process(Mat &io_img);

private:
    void drawText(Mat &io_img, const String &text);

    FpsCounter m_counter;
};

}    // namespace cv
