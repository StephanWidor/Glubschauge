#pragma once
#include <opencv2/highgui.hpp>

namespace cv {

class ImageUtils
{
public:
    static double fitToSize(const Mat &img, Size size, bool inner, Mat &o_fitImg);

    static cv::Mat fitToSize(const Mat &img, Size size, bool inner);

    struct BarrelInfo
    {
        Point center;
        double radius = 0.0;
        double power = 0.0;
    };

    static Rect boundingRect(const BarrelInfo &info)
    {
        const auto &center = info.center;
        const auto radius = static_cast<int>(std::ceil(info.radius));
        return cv::Rect(center.x - radius, center.y - radius, 2 * radius, 2 * radius);
    }

    static void distort(Mat &io_img, const BarrelInfo &barrelInfo);

    static void distort(Mat &io_img, const std::vector<BarrelInfo> &barrelInfos)
    {
        for (const auto &info : barrelInfos)
            distort(io_img, info);
    }

    static char showDebug(const Mat &img, const std::string *pWindowName = nullptr);
};

}    // namespace cv
