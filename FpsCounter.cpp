#include "FpsCounter.h"

using namespace std::chrono;

void FpsCounter::setEnabled(bool enabled)
{
    if (enabled == m_enabled)
        return;
    if (enabled)
    {
        m_tickCount = 0u;
        m_fps = 0.0;
        m_startTime = system_clock::now();
        m_enabled = true;
    }
    else
    {
        m_fps = -1.0;
        m_enabled = false;
    }
}

double FpsCounter::tick(const std::function<void(double)> &callbackAfterEval)
{
    if (m_enabled)
    {
        using namespace std::chrono;
        const auto diffTime = duration_cast<milliseconds>(system_clock::now() - m_startTime);
        if (diffTime < m_evalTime)
            ++m_tickCount;
        else
        {
            m_fps = (diffTime / m_evalTime) * static_cast<double>(m_tickCount);
            if (m_fps > 0.0)
                callbackAfterEval(m_fps);
            m_tickCount = 0u;
            m_startTime = system_clock::now();
        }
    }
    return m_fps;
}
