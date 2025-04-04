#include "qt/ProcessingFilter.h"
#include "qt/Assets.h"
#include "qt/FileSystem.h"
#include <QFile>
#include <cv/ImageUtils.h>
#include <logger.h>
#include <thread>

qt::ProcessingFilter::ProcessingFilter(QObject *pParent)
    : QObject(pParent)
    , m_glubschEffect(Assets::provideCascadeData().string(), Assets::provideFacemarkData().string(),
                      cv::loadGlubschConfigFromYaml(FileSystem::glubschConfigPath()))
{
    connect(&m_inputSocket.sink, &QVideoSink::videoFrameChanged, this, &ProcessingFilter::onInputFrame);
    connect(&m_imageCapture, &qt::ImageCapture::stateChanged, [this](ImageCapture::State) {
        emit collectingCaptureChanged();
        emit processingCaptureChanged();
        emit busyCaptureChanged();
    });

    m_processing.store(true, std::memory_order::release);
    std::thread processingThread([&]() {
        while (m_processingThreadShouldRun)
        {
            auto img = m_inputSocket.get();
            if (img.empty())
                continue;

            m_glubschEffect.process(img);
            m_imageCapture.push(img);
            m_flashEffect.process(img);
            if (m_outputMirrored)
                cv::flip(img, img, 1);
            m_fpsEffect.process(img);
            m_v4lSocket.push(img);

            m_outputSocket.push(img);
        }
        m_processing.store(false, std::memory_order::release);
    });
    processingThread.detach();
}

qt::ProcessingFilter::~ProcessingFilter()
{
    cv::saveToYaml(m_glubschEffect.config, FileSystem::glubschConfigPath());
    m_processingThreadShouldRun = false;
    disconnect(&m_inputSocket.sink, &QVideoSink::videoFrameChanged, this, &ProcessingFilter::onInputFrame);
    while (m_processing.load(std::memory_order::acquire))
        ;
}

void qt::ProcessingFilter::onInputFrame(const QVideoFrame &frame)
{
    m_inputSocket.push(frame);
}

void qt::ProcessingFilter::capture(const CaptureType type)
{
    switch (type)
    {
        case JPG:
        {
            m_flashEffect.trigger();
            m_imageCapture.startJPG();
            break;
        }
        case GIF:
        {
            m_imageCapture.startGIF(std::chrono::milliseconds{2000u});
            break;
        }
    }
}

void qt::ProcessingFilter::setShowLandmarks(bool show)
{
    if (m_glubschEffect.config.drawLandmarks != show)
    {
        m_glubschEffect.config.drawLandmarks = show;
        emit showLandmarksChanged();
    }
}

void qt::ProcessingFilter::setDistortAlways(bool distort)
{
    if (m_glubschEffect.config.distortAlways != distort)
        m_glubschEffect.config.distortAlways = distort;
}

void qt::ProcessingFilter::setShowFps(bool show)
{
    if (m_fpsEffect.enabled() != show)
    {
        m_fpsEffect.setEnabled(show);
        emit showFpsChanged();
    }
}

void qt::ProcessingFilter::setDistort(cv::FaceDistortionType type, double factor)
{
    if (getDistort(type) != factor)
    {
        cv::set(m_glubschEffect.config.distortions, type, factor);
        emit distortChanged();
    }
}

void qt::ProcessingFilter::saveConfig()
{
    cv::saveToYaml(m_glubschEffect.config, FileSystem::glubschConfigPath());
}

void qt::ProcessingFilter::InputSocket::push(const QVideoFrame &frame)
{
    if (!frame.isValid())
    {
        logger::out << "frame is invalid";
        return;
    }

    std::lock_guard lock(mutex);

    if (QVideoFrame copyFrame(frame);    // shallow copy
        copyFrame.map(QVideoFrame::ReadOnly))    // (map is a const member function)
    {
        img = copyFrame.toImage().convertToFormat(QImage::Format_BGR888);
        copyFrame.unmap();
        newImgAvailable = true;
    }
    else    // TODO: investigate why this happens on android when getting input from MediaPlayer
        logger::out << "couldn't map video frame";
}

cv::Mat qt::ProcessingFilter::InputSocket::get()
{
    std::lock_guard lock(mutex);
    if (newImgAvailable)
    {
        newImgAvailable = false;
        return cv::Mat(img.height(), img.width(), CV_8UC3, img.bits(), img.bytesPerLine()).clone();
    }
    return cv::Mat();
}

void qt::ProcessingFilter::OutputSocket::push(const cv::Mat &img)
{
    if (mutex.try_lock())
    {
        if (pSink)
            pSink->setVideoFrame(QVideoFrame(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_BGR888)));
        mutex.unlock();
    }
}

void qt::ProcessingFilter::OutputSocket::setSink(QVideoSink *pNewSink)
{
    if (pNewSink == pSink)
        return;
    std::lock_guard lock(mutex);
    pSink = pNewSink;
}

bool qt::ProcessingFilter::V4lSocket::setDevice(const std::string_view device)
{
    std::lock_guard lock(mutex);
    pDevice = std::make_unique<cv::V4lLoopbackDevice>(device);
    if (!pDevice->ok())
        pDevice = nullptr;
    return true;
}

void qt::ProcessingFilter::V4lSocket::clearDevice()
{
    std::lock_guard lock(mutex);
    pDevice = nullptr;
}

void qt::ProcessingFilter::V4lSocket::push(const cv::Mat &img)
{
    std::lock_guard lock(mutex);
    if (pDevice)
        pDevice->push(img);
}
