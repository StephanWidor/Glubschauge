#pragma once
#include "cv/Utils2D.h"

#include <opencv2/face.hpp>
#include <opencv2/opencv.hpp>

namespace cv {

class FaceDetection
{
public:
    using BBoxes = std::vector<Rect>;
    using Landmarks = std::vector<Point2f>;

    enum class LandmarksType
    {
        JawLine,
        LeftEyebrow,
        RightEyebrow,
        NoseBridge,
        LowerNose,
        LeftEye,
        RightEye,
        OuterLip,
        InnerLip
    };

    static Landmarks filter(const Landmarks &landmarks68, LandmarksType);

    template<typename... LandmarksTypes>
    static Landmarks filter(const Landmarks &landmarks68, LandmarksType type, LandmarksTypes... types)
    {
        auto landmarks0 = filter(landmarks68, type);
        auto landmarks1 = filter(landmarks68, types...);
        landmarks0.insert(landmarks0.end(), landmarks1.begin(), landmarks1.end());
        return landmarks0;
    }

    FaceDetection(): m_dataLoaded(false), m_facemark(face::FacemarkLBF::create()) { m_dataLoaded = loadData(); }

    std::pair<BBoxes, std::vector<Landmarks>> detect(const Mat &img);

private:
    bool loadData();

    std::vector<Rect> findBBoxes(const Mat &img);

    ContourVector getLandmarkContours(const std::vector<Point2f> &landmarks);

    bool m_dataLoaded = false;
    CascadeClassifier m_faceDetector;
    Ptr<face::Facemark> m_facemark;
};

}    // namespace cv
