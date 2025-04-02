#pragma once
#include <opencv2/imgproc.hpp>

namespace cv {
using Contour = std::vector<Point>;
using ContourVector = std::vector<Contour>;

class Utils2D
{
public:
    template<typename Scalar>
    static Point round(const Point_<Scalar> &point)
    {
        return Point(std::round(point.x), std::round(point.y));
    }

    static cv::Rect scale(const cv::Rect &rect, double factor)
    {
        return cv::Rect(
          std::round(factor * static_cast<double>(rect.x)), std::round(factor * static_cast<double>(rect.y)),
          std::round(factor * static_cast<double>(rect.width)), std::round(static_cast<double>(factor * rect.height)));
    }

    static Point center(const cv::Rect &rect)
    {
        return Point(rect.x + std::round(0.5 * static_cast<double>(rect.width)),
                     rect.y + std::round(0.5 * static_cast<double>(rect.height)));
    }

    static Contour toContour(const cv::Rect &rect)
    {
        return {Point(rect.br().x, rect.tl().y), rect.tl(), Point(rect.tl().x, rect.br().y), rect.br()};
    }

    static ContourVector toContours(const std::vector<cv::Rect> &rects)
    {
        ContourVector boxContours;
        std::transform(rects.begin(), rects.end(), std::back_inserter(boxContours),
                       [](const auto &box) { return Utils2D::toContour(box); });
        return boxContours;
    }
};

}    // namespace cv
