#include "qt/ProcessingFilter.h"
#include "FileSystem.h"
#include "Logger.h"
#include "qt/ImageConvert.h"

#include <thread>

QVideoFrame qt::ProcessingFilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &, RunFlags)
{
    cv::Mat img = ImageConvert::toMat(*input);
    m_filter.m_imgTransform.transformFromCamera(img);
    m_filter.m_glubschEffect.process(img);
    if (m_filter.m_captureNext)
        capture(img);
    if (m_filter.m_gifCreator.collecting())
        m_filter.m_gifCreator.push(img);
    m_filter.m_flashEffect.process(img);
    m_filter.m_fpsEffect.process(img);
    if (m_filter.streamingToOutputDevice())
        m_filter.m_pOutputDevice->push(img);
    m_filter.m_imgTransform.transformToCamera(img);
    if (QVideoFrame output = ImageConvert::toQVideoFrame(img); output.width() != 0)
        return output;
    return *input;
}

void qt::ProcessingFilterRunnable::capture(const cv::Mat &img)
{
    std::thread(
      [&](cv::Mat saveImg) {
          const auto path = FileSystem::generatePathForNewPicture();
          if (FileSystem::requestPermission(FileSystem::AccessType::Write) && cv::imwrite(path, saveImg))
          {
              m_filter.m_flashEffect.trigger();
              FileSystem::triggerMediaScan(path);
          }
      },
      img.clone())
      .detach();
    m_filter.m_captureNext = false;
}

QVideoFilterRunnable *qt::ProcessingFilter::createFilterRunnable()
{
    m_pRunnable = new ProcessingFilterRunnable(*this);
    return m_pRunnable;
}

void qt::ProcessingFilter::captureGif()
{
    if (FileSystem::requestPermission(FileSystem::AccessType::Write))
    {
        const auto path = FileSystem::generatePathForNewPicture("gif");
        m_gifCreator.start(
          path, std::chrono::milliseconds{2000u},
          [this]() {
              emit capturingGifChanged();
              emit processingGifChanged();
          },
          [this, path]() {
              FileSystem::triggerMediaScan(path);
              emit processingGifChanged();
          });
        emit capturingGifChanged();
    }
    else
        logger::out << "No Permission to write gif";
}

void qt::ProcessingFilter::setShowLandmarks(bool show)
{
    if (m_glubschEffect.getDrawLandmarks() != show)
    {
        m_glubschEffect.setDrawLandmarks(show);
        emit showLandmarksChanged();
    }
}

void qt::ProcessingFilter::setDoDistort(cv::FaceDistortionType type, bool distort)
{
    if (m_glubschEffect.getDoDistort(type) != distort)
    {
        m_glubschEffect.setDoDistort(type, distort);
        emit doDistortChanged();
    }
}

void qt::ProcessingFilter::setShowFps(bool show)
{
    if (m_fpsEffect.enabled() != show)
    {
        m_fpsEffect.setEnabled(show);
        emit showFpsChanged();
    }
}

void qt::ProcessingFilter::streamToOutputDevice(const QString &device)
{
    if (m_pOutputDevice == nullptr)
    {
        m_pOutputDevice = std::make_unique<cv::OutputDevice>(device.toStdString().c_str());
        emit streamingToOutputDeviceChanged();
    }
}

void qt::ProcessingFilter::stopStreamingToOutputDevice()
{
    if (m_pOutputDevice != nullptr)
    {
        m_pOutputDevice = nullptr;
        emit streamingToOutputDeviceChanged();
    }
}
