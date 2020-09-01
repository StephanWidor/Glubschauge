#pragma once
#include <chrono>
#include <functional>

class FpsCounter
{
public:
    void setEnabled(bool enabled);

    bool enabled() const { return m_enabled; }

    double tick(const std::function<void(double)> &callbackAfterEval = [](double) {});

private:
    bool m_enabled = false;
    unsigned m_tickCount = 0u;
    double m_fps = -1.0;
    std::chrono::time_point<std::chrono::system_clock> m_startTime;
    const std::chrono::milliseconds m_evalTime{1000};
};
