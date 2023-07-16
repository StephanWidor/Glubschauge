#pragma once
#include "qt/ImageTransform.h"
#include <QAbstractVideoFilter>
#include <cv/FlashEffect.h>
#include <cv/FpsEffect.h>
#include <cv/GifCreate.h>
#include <cv/GlubschEffect.h>
#include <cv/OutputDevice.h>

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
    Q_PROPERTY(double eyesDistort READ getEyesDistort WRITE setEyesDistort NOTIFY distortChanged)
    Q_PROPERTY(double noseDistort READ getNoseDistort WRITE setNoseDistort NOTIFY distortChanged)
    Q_PROPERTY(double mouthDistort READ getMouthDistort WRITE setMouthDistort NOTIFY distortChanged)
    Q_PROPERTY(double upperHeadDistort READ getUpperHeadDistort WRITE setUpperHeadDistort NOTIFY distortChanged)
    Q_PROPERTY(double lowerHeadDistort READ getLowerHeadDistort WRITE setLowerHeadDistort NOTIFY distortChanged)
    Q_PROPERTY(bool distortAlways READ getDistortAlways WRITE setDistortAlways NOTIFY distortChanged)
    Q_PROPERTY(bool showFps READ getShowFps WRITE setShowFps NOTIFY showFpsChanged)
    Q_PROPERTY(bool gifEnabled READ gifEnabled CONSTANT)
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
    void distortChanged();
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

    void setEyesDistort(double factor) { setDistort(cv::FaceDistortionType::Eyes, factor); }

    double getEyesDistort() const { return m_glubschEffect.getDistort(cv::FaceDistortionType::Eyes); }

    void setNoseDistort(double factor) { setDistort(cv::FaceDistortionType::Nose, factor); }

    double getNoseDistort() const { return m_glubschEffect.getDistort(cv::FaceDistortionType::Nose); }

    void setMouthDistort(double factor) { setDistort(cv::FaceDistortionType::Mouth, factor); }

    void setDistortAlways(bool distort) { m_glubschEffect.setDistortAlways(distort); }

    double getMouthDistort() const { return m_glubschEffect.getDistort(cv::FaceDistortionType::Mouth); }

    void setUpperHeadDistort(double factor) { setDistort(cv::FaceDistortionType::UpperHead, factor); }

    double getUpperHeadDistort() const { return m_glubschEffect.getDistort(cv::FaceDistortionType::UpperHead); }

    void setLowerHeadDistort(double factor) { setDistort(cv::FaceDistortionType::LowerHead, factor); }

    double getLowerHeadDistort() const { return m_glubschEffect.getDistort(cv::FaceDistortionType::LowerHead); }

    bool getDistortAlways() const { return m_glubschEffect.getDistortAlways(); }

    void setDistort(cv::FaceDistortionType type, double factor);

    void setShowFps(bool show);

    bool getShowFps() const { return m_fpsEffect.enabled(); }

    static bool gifEnabled() { return cv::GifCreate::implemented(); }

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
    static constexpr bool m_streamingToOutputPossible = cv::OutputDevice::implemented();
    std::unique_ptr<cv::OutputDevice> m_pOutputDevice;
};

}    // namespace qt
