#include "FpsCounter.h"

void FpsCounter::start(const std::function<void(double)> &callbackAfterEval, std::chrono::milliseconds evalTime)
{
    if (!m_running)
    {
        m_tickCount = 0u;
        m_fps = 0.0;
        m_evalTime = evalTime;
        m_callbackAfterEval = callbackAfterEval;
        m_startTime = std::chrono::system_clock::now();
        m_running = true;
    }
}

void FpsCounter::stop()
{
    if (m_running)
    {
        m_fps = -1.0;
        m_running = false;
    }
}

double FpsCounter::tick()
{
    if (m_running)
    {
        using namespace std::chrono;
        const auto diffTime = duration_cast<milliseconds>(system_clock::now() - m_startTime);
        if (diffTime < m_evalTime)
            ++m_tickCount;
        else
        {
            m_fps = (diffTime / m_evalTime) * static_cast<double>(m_tickCount);
            if (m_fps > 0.0)
                m_callbackAfterEval(m_fps);
            m_tickCount = 0u;
            m_startTime = system_clock::now();
        }
    }
    return m_fps;
}
