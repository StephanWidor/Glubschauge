#include "qt/ImageCapture.h"
#include "qt/FileSystem.h"
#include <QThread>
#include <ctime>
#include <cv/ImageUtils.h>
#include <format>
#include <logger.h>
#include <sstream>

namespace qt {

namespace {

std::filesystem::path generateFilePath(const std::string_view fileEnding)
{
    const auto t = std::time(nullptr);
    const auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y%m%d_%H%M%S");
    return picturesDir() / std::format("glubsch_{}.{}", oss.str(), fileEnding);
}

}    // namespace

ImageCapture::~ImageCapture()
{
    // busy wait until gif saving has finished
    while (state() != Idle)
        ;
}

void ImageCapture::startJPG()
{
    if (state() == Idle)
        setState(CollectingJPG);
}

void ImageCapture::startGIF(std::chrono::milliseconds duration)
{
    if (state() == Idle)
    {
        m_gifDuration = duration;
        m_startTime = std::chrono::system_clock::now();
        setState(CollectingGIF);
    }
}

void ImageCapture::push(const cv::TimeStampedImg &img)
{
    if (state() >= CollectingJPG)
    {
        switch (state())
        {
            case CollectingJPG:
            {
                setState(ProcessingJPG);
                // we use QThread because moveToPictures uses QFileDialog, which is unhappy being started from std::thread
                auto qThread = QThread::create([this, imgCopy = img.first.clone()]() {
                    const auto filePath = generateFilePath("jpg");
                    if (!cv::imwrite(filePath.string(), imgCopy))
                        logger::out << std::format("failed to save {}", filePath.string());
#ifdef ANDROID
                    moveToUserChoiceDir(filePath);
#endif
                    setState(Idle);
                });
                qThread->start();
                break;
            }
            case CollectingGIF:
            {
                m_images.push_back({img.first.clone(), img.second});
                if (const auto diffTime =
                      duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - m_startTime);
                    diffTime >= m_gifDuration)
                {
                    setState(ProcessingGIF);
                    auto qThread = QThread::create([diffTime, this]() {
                        const auto filePath = generateFilePath("gif");
                        if (!cv::saveGif(m_images, filePath))
                            logger::out << std::format("failed to save {}", filePath.string());
                        m_images.clear();
#ifdef ANDROID
                        moveToUserChoiceDir(filePath);
#endif
                        setState(Idle);
                    });
                    qThread->start();
                }
                break;
            }
            default:
                break;
        }
    }
}

}    // namespace qt
