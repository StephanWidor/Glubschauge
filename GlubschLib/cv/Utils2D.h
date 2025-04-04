#pragma once
#include <opencv2/imgproc.hpp>

namespace cv {
using Contour = std::vector<Point>;
using ContourVector = std::vector<Contour>;

template<typename Scalar>
Point round(const Point_<Scalar> &point)
{
    return Point(std::round(point.x), std::round(point.y));
}

inline cv::Rect scale(const cv::Rect &rect, double factor)
{
    return cv::Rect(std::round(factor * static_cast<double>(rect.x)), std::round(factor * static_cast<double>(rect.y)),
                    std::round(factor * static_cast<double>(rect.width)),
                    std::round(static_cast<double>(factor * rect.height)));
}

inline Point center(const cv::Rect &rect)
{
    return Point(rect.x + std::round(0.5 * static_cast<double>(rect.width)),
                 rect.y + std::round(0.5 * static_cast<double>(rect.height)));
}

inline Contour toContour(const cv::Rect &rect)
{
    return {Point(rect.br().x, rect.tl().y), rect.tl(), Point(rect.tl().x, rect.br().y), rect.br()};
}

inline ContourVector toContours(const std::vector<cv::Rect> &rects)
{
    ContourVector boxContours;
    std::transform(rects.begin(), rects.end(), std::back_inserter(boxContours),
                   [](const auto &box) { return toContour(box); });
    return boxContours;
}

}    // namespace cv
