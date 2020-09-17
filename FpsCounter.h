#pragma once
#include <chrono>
#include <functional>

class FpsCounter
{
public:
    void start(
      const std::function<void(double)> &callbackAfterEval = [](double) {},
      std::chrono::milliseconds evalTime = std::chrono::milliseconds{1000u});

    void stop();

    void setEnabled(bool enabled);

    bool running() const { return m_running; }

    double tick();

private:
    bool m_running = false;
    unsigned m_tickCount = 0u;
    double m_fps = -1.0;
    std::chrono::time_point<std::chrono::system_clock> m_startTime;
    std::chrono::milliseconds m_evalTime{1000};
    std::function<void(double)> m_callbackAfterEval = [](double) {};
};
