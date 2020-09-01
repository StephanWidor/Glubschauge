#include "qt/ProcessingFilter.h"
#include "Logger.h"
#include "magick/GifCreate.h"
#include "qt/FileSystem.h"
#include "qt/ImageConvert.h"

#include <chrono>

QVideoFrame qt::ProcessingFilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &, RunFlags)
{
    cv::Mat img = ImageConvert::toMat(*input);
    m_filter.m_imgTransform.transformFromCamera(img);
    m_filter.m_glubschEffect.process(img);
    if (m_filter.m_captureNext)
        capture(img);
    if (m_filter.m_captureGif)
        captureGif(img);
    m_filter.m_flashEffect.process(img);
    m_filter.m_fpsEffect.process(img);
    m_filter.m_imgTransform.transformToCamera(img);
    if (QVideoFrame output = ImageConvert::toQVideoFrame(img); output.width() != 0)
        return output;
    return *input;
}

void qt::ProcessingFilterRunnable::capture(const cv::Mat &img)
{
    const auto path = qt::FileSystem::generatePathForNewPicture();
    if (cv::imwrite(path, img))
        m_filter.m_flashEffect.trigger();
    m_filter.m_captureNext = false;
}

void qt::ProcessingFilterRunnable::captureGif(const cv::Mat &img)
{
#ifdef ANDROID
    (void)img;
    m_filter.m_captureGif = false;
    m_filter.capturingGifChanged();
#else
    if (m_filter.m_gifCreator.size() < 30)
        m_filter.m_gifCreator.push(img);
    else
    {
        m_filter.m_gifCreator.save(qt::FileSystem::generatePathForNewPicture("gif"));
        m_filter.m_captureGif = false;
        m_filter.capturingGifChanged();
    }
#endif
}
