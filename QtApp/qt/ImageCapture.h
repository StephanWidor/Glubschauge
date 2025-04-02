#pragma once
#include <QObject>
#include <QString>
#include <atomic>
#include <chrono>
#include <magick/GifContainer.h>

namespace qt {

class ImageCapture final : public QObject
{
    Q_OBJECT
public:
    ImageCapture(): QObject() {}

    ~ImageCapture() override;

    static constexpr bool gifImplemented() { return magick::GifContainer::implemented(); }

    void startJPG();

    void startGIF(std::chrono::milliseconds duration);

    void push(const cv::Mat &img);

    enum State : std::uint8_t
    {
        Idle = 0,
        CollectingJPG = 1,
        CollectingGIF = 2,
        ProcessingJPG = 4,
        ProcessingGIF = 8,
        Collecting = CollectingJPG | CollectingGIF,
        Processing = ProcessingJPG | ProcessingGIF,
        Busy = std::numeric_limits<std::uint8_t>::max()
    };
    Q_ENUM(State);

    State state() const { return m_state.load(std::memory_order::acquire); }

    bool idle() const { return state() == Idle; }

    bool busy() const { return state() & Busy; }

    bool collecting() const { return state() & Collecting; }

    bool processing() const { return state() & Processing; }

signals:
    void stateChanged(State) const;

private:
    void setState(const State state)
    {
        m_state.store(state, std::memory_order::release);
        emit stateChanged(state);
    }

    std::atomic<State> m_state{State::Idle};
    std::chrono::time_point<std::chrono::system_clock> m_startTime;
    std::chrono::milliseconds m_gifDuration{0};
    magick::GifContainer m_gifContainer;
};

}    // namespace qt
