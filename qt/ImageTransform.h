#pragma once
#include "Logger.h"
#include "cv/ImageUtils.h"
#include <QScreen>

namespace qt {

class ImageTransform : public QObject
{
    Q_OBJECT

public:
    void setRotation(int rotation) { m_rotation = rotation; }

    int getRotation() const { return m_rotation; }

    void setDoCameraTransform(bool flip) { m_doCameraTransform = flip; }

    int getDoCameraTransform() const { return m_doCameraTransform; }

    void transformFromCamera(cv::Mat &) const;

    void transformToCamera(cv::Mat &) const;

private:
    static void rotate(cv::Mat &io_img, int rotation);

    bool m_doCameraTransform = true;
    std::atomic<int> m_rotation{0};
};

}    // namespace qt
