#pragma once
#include "qt/ImageCapture.h"
#include <QUrl>
#include <QVideoFrame>
#include <QVideoSink>
#include <cv/FlashEffect.h>
#include <cv/FpsEffect.h>
#include <cv/GlubschEffect.h>
#include <cv/V4lLoopbackDevice.h>
#include <spin_mutex.h>

namespace qt {

class ProcessingFilter final : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool outputMirrored READ getOutputMirrored WRITE setOutputMirrored NOTIFY outputMirroredChanged)
    Q_PROPERTY(bool showLandmarks READ getShowLandmarks WRITE setShowLandmarks NOTIFY showLandmarksChanged)
    Q_PROPERTY(double eyesDistort READ getEyesDistort WRITE setEyesDistort NOTIFY distortChanged)
    Q_PROPERTY(double noseDistort READ getNoseDistort WRITE setNoseDistort NOTIFY distortChanged)
    Q_PROPERTY(double mouthDistort READ getMouthDistort WRITE setMouthDistort NOTIFY distortChanged)
    Q_PROPERTY(double upperHeadDistort READ getUpperHeadDistort WRITE setUpperHeadDistort NOTIFY distortChanged)
    Q_PROPERTY(double lowerHeadDistort READ getLowerHeadDistort WRITE setLowerHeadDistort NOTIFY distortChanged)
    Q_PROPERTY(bool distortAlways READ getDistortAlways WRITE setDistortAlways NOTIFY distortChanged)
    Q_PROPERTY(bool showFps READ getShowFps WRITE setShowFps NOTIFY showFpsChanged)
    Q_PROPERTY(bool gifEnabled READ gifEnabled CONSTANT)
    Q_PROPERTY(bool collectingCapture READ collectingCapture NOTIFY collectingCaptureChanged)
    Q_PROPERTY(bool processingCapture READ processingCapture NOTIFY processingCaptureChanged)
    Q_PROPERTY(bool busyCapture READ busyCapture NOTIFY busyCaptureChanged)
    Q_PROPERTY(bool v4lOutputPossible READ v4lOutputPossible CONSTANT)
    Q_PROPERTY(bool v4lDeviceActice READ v4lDeviceActive NOTIFY v4lDeviceActiveChanged)

public:
    ProcessingFilter(QObject *pParent = nullptr);

    ~ProcessingFilter() override;

    enum CaptureType
    {
        JPG,
        GIF
    };
    Q_ENUM(CaptureType);

signals:
    void outputMirroredChanged();
    void showLandmarksChanged();
    void distortChanged();
    void showFpsChanged();
    void collectingCaptureChanged();
    void processingCaptureChanged();
    void busyCaptureChanged();
    void v4lDeviceActiveChanged();

public slots:
    QVideoSink *videoSink() { return &m_inputSocket.sink; }

    void setOutputSink(QVideoSink *pSink) { m_outputSocket.setSink(pSink); }

    void capture(CaptureType);

    void setV4lDevice(const QString &device)
    {
        m_v4lSocket.setDevice(device.toStdString());
        emit v4lDeviceActiveChanged();
    }

    void clearV4lDevice()
    {
        m_v4lSocket.clearDevice();
        emit v4lDeviceActiveChanged();
    }

    void saveConfig();

private:
    void onInputFrame(const QVideoFrame &frame);

    void setOutputMirrored(bool outputMirrored) { m_outputMirrored = outputMirrored; }

    bool getOutputMirrored() const { return m_outputMirrored; }

    void setShowLandmarks(bool show);

    bool getShowLandmarks() const { return m_glubschEffect.config.drawLandmarks; }

    void setDistortAlways(bool distort);

    bool getDistortAlways() const { return m_glubschEffect.config.distortAlways; }

    void setShowFps(bool show);

    bool getShowFps() const { return m_fpsEffect.enabled(); }

    void setDistort(cv::FaceDistortionType type, double factor);

    double getDistort(cv::FaceDistortionType type) const { return cv::get(m_glubschEffect.config.distortions, type); }

    void setEyesDistort(double factor) { setDistort(cv::FaceDistortionType::Eyes, factor); }

    double getEyesDistort() const { return getDistort(cv::FaceDistortionType::Eyes); }

    void setNoseDistort(double factor) { setDistort(cv::FaceDistortionType::Nose, factor); }

    double getNoseDistort() const { return getDistort(cv::FaceDistortionType::Nose); }

    void setMouthDistort(double factor) { setDistort(cv::FaceDistortionType::Mouth, factor); }

    double getMouthDistort() const { return getDistort(cv::FaceDistortionType::Mouth); }

    void setUpperHeadDistort(double factor) { setDistort(cv::FaceDistortionType::UpperHead, factor); }

    double getUpperHeadDistort() const { return getDistort(cv::FaceDistortionType::UpperHead); }

    void setLowerHeadDistort(double factor) { setDistort(cv::FaceDistortionType::LowerHead, factor); }

    double getLowerHeadDistort() const { return getDistort(cv::FaceDistortionType::LowerHead); }

    static bool gifEnabled() { return qt::ImageCapture::gifImplemented(); }

    bool collectingCapture() const { return m_imageCapture.collecting(); }

    bool processingCapture() const { return m_imageCapture.processing(); }

    bool busyCapture() const { return m_imageCapture.busy(); }

    std::atomic<bool> m_processing{false};
    bool m_processingThreadShouldRun = true;

    cv::GlubschEffect m_glubschEffect;
    cv::FlashEffect m_flashEffect;
    cv::FpsEffect m_fpsEffect;
    qt::ImageCapture m_imageCapture;

    bool m_captureNext = false;
    bool m_outputMirrored = false;

    struct InputSocket
    {
        void push(const QVideoFrame &);
        cv::Mat get();

        QVideoSink sink;
        QImage img;
        bool newImgAvailable = false;
        spin_mutex mutex;
    };
    InputSocket m_inputSocket;

    struct OutputSocket
    {
        void setSink(QVideoSink *pNewSink);
        void push(const cv::Mat &img);

        spin_mutex mutex;
        QVideoSink *pSink = nullptr;
    };
    OutputSocket m_outputSocket;

    struct V4lSocket
    {
        bool setDevice(std::string_view device);

        void clearDevice();

        void push(const cv::Mat &img);

        spin_mutex mutex;
        std::unique_ptr<cv::V4lLoopbackDevice> pDevice;
    };
    V4lSocket m_v4lSocket;

    static constexpr bool v4lOutputPossible() { return cv::V4lLoopbackDevice::implemented(); }
    bool v4lDeviceActive() const { return m_v4lSocket.pDevice != nullptr; }
};

}    // namespace qt
