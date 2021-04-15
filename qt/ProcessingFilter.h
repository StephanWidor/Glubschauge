#pragma once
#include "cv/FlashEffect.h"
#include "cv/FpsEffect.h"
#include "cv/GifCreate.h"
#include "cv/GlubschEffect.h"
#include "cv/OutputDevice.h"
#include "qt/ImageTransform.h"
#include <QAbstractVideoFilter>

namespace qt {

class ProcessingFilter;

class ProcessingFilterRunnable : public QVideoFilterRunnable
{
public:
    ProcessingFilterRunnable(ProcessingFilter &filter): QVideoFilterRunnable(), m_filter(filter) {}

    QVideoFrame run(QVideoFrame *, const QVideoSurfaceFormat &, RunFlags) override;

private:
    void capture(const cv::Mat &img);

    ProcessingFilter &m_filter;
};

class ProcessingFilter : public QAbstractVideoFilter
{
    Q_OBJECT
    Q_PROPERTY(int rotation READ getRotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(
      bool doCameraTransform READ getDoCameraTransform WRITE setDoCameraTransform NOTIFY doCameraTransformChanged)
    Q_PROPERTY(bool showLandmarks READ getShowLandmarks WRITE setShowLandmarks NOTIFY showLandmarksChanged)
    Q_PROPERTY(bool doEyesDistort READ getDoEyesDistort WRITE setDoEyesDistort NOTIFY doDistortChanged)
    Q_PROPERTY(bool doNoseDistort READ getDoNoseDistort WRITE setDoNoseDistort NOTIFY doDistortChanged)
    Q_PROPERTY(bool doMouthDistort READ getDoMouthDistort WRITE setDoMouthDistort NOTIFY doDistortChanged)
    Q_PROPERTY(bool doUpperHeadDistort READ getDoUpperHeadDistort WRITE setDoUpperHeadDistort NOTIFY doDistortChanged)
    Q_PROPERTY(bool doLowerHeadDistort READ getDoLowerHeadDistort WRITE setDoLowerHeadDistort NOTIFY doDistortChanged)
    Q_PROPERTY(bool distortAlways READ getDistortAlways WRITE setDistortAlways NOTIFY doDistortChanged)
    Q_PROPERTY(bool showFps READ getShowFps WRITE setShowFps NOTIFY showFpsChanged)
    Q_PROPERTY(bool gifEnabled MEMBER m_gifEnabled CONSTANT)
    Q_PROPERTY(bool capturingGif READ capturingGif NOTIFY capturingGifChanged)
    Q_PROPERTY(bool processingGif READ processingGif NOTIFY processingGifChanged)
    Q_PROPERTY(bool streamingToOutputPossible MEMBER m_streamingToOutputPossible CONSTANT)
    Q_PROPERTY(bool streamingToOutputDevice READ streamingToOutputDevice NOTIFY streamingToOutputDeviceChanged)

    friend ProcessingFilterRunnable;

public:
    ProcessingFilter(QObject *pParent = nullptr);

    QVideoFilterRunnable *createFilterRunnable() override;

signals:
    void rotationChanged();
    void doCameraTransformChanged();
    void showLandmarksChanged();
    void doDistortChanged();
    void showFpsChanged();
    void capturingGifChanged();
    void processingGifChanged();
    void streamingToOutputDeviceChanged();

public slots:

    void capture() { m_captureNext = true; }

    void captureGif();

    void streamToOutputDevice(const QString &device);

    void stopStreamingToOutputDevice();

protected:
    void setRotation(int rotation) { m_imgTransform.setRotation(rotation); }

    int getRotation() const { return m_imgTransform.getRotation(); }

    void setDoCameraTransform(bool doTransform) { m_imgTransform.setDoCameraTransform(doTransform); }

    bool getDoCameraTransform() const { return m_imgTransform.getDoCameraTransform(); }

    void setShowLandmarks(bool show);

    bool getShowLandmarks() const { return m_glubschEffect.getDrawLandmarks(); }

    void setDoEyesDistort(bool distort) { setDoDistort(cv::FaceDistortionType::Eyes, distort); }

    bool getDoEyesDistort() const { return m_glubschEffect.getDoDistort(cv::FaceDistortionType::Eyes); }

    void setDoNoseDistort(bool distort) { setDoDistort(cv::FaceDistortionType::Nose, distort); }

    bool getDoNoseDistort() const { return m_glubschEffect.getDoDistort(cv::FaceDistortionType::Nose); }

    void setDoMouthDistort(bool distort) { setDoDistort(cv::FaceDistortionType::Mouth, distort); }

    void setDistortAlways(bool distort) { m_glubschEffect.setDistortAlways(distort); }

    bool getDoMouthDistort() const { return m_glubschEffect.getDoDistort(cv::FaceDistortionType::Mouth); }

    void setDoUpperHeadDistort(bool distort) { setDoDistort(cv::FaceDistortionType::UpperHead, distort); }

    bool getDoUpperHeadDistort() const { return m_glubschEffect.getDoDistort(cv::FaceDistortionType::UpperHead); }

    void setDoLowerHeadDistort(bool distort) { setDoDistort(cv::FaceDistortionType::LowerHead, distort); }

    bool getDoLowerHeadDistort() const { return m_glubschEffect.getDoDistort(cv::FaceDistortionType::LowerHead); }

    bool getDistortAlways() const { return m_glubschEffect.getDistortAlways(); }

    void setDoDistort(cv::FaceDistortionType type, bool distort);

    void setShowFps(bool show);

    bool getShowFps() const { return m_fpsEffect.enabled(); }

    bool capturingGif() const { return m_gifCreator.collecting(); }

    bool processingGif() const { return m_gifCreator.processing(); }

    bool streamingToOutputDevice() const { return m_pOutputDevice != nullptr; }

    ImageTransform m_imgTransform;
    cv::GlubschEffect m_glubschEffect;
    cv::FlashEffect m_flashEffect;
    cv::FpsEffect m_fpsEffect;
    cv::GifCreate m_gifCreator;
    ProcessingFilterRunnable *m_pRunnable = nullptr;
    bool m_captureNext = false;
#ifdef IMAGEMAGICK_AVAILABLE
    static constexpr bool m_gifEnabled = true;
#else
    static constexpr bool m_gifEnabled = false;
#endif
    static constexpr bool m_streamingToOutputPossible = cv::OutputDevice::implemented();
    std::unique_ptr<cv::OutputDevice> m_pOutputDevice;
};

}    // namespace qt
