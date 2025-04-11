#pragma once
#include <filesystem>
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

    FaceDetection(const std::filesystem::path &cascadeDataPath, const std::filesystem::path &facemarkDataPath);

    std::pair<BBoxes, std::vector<Landmarks>> detect(const Mat &img);

    static Landmarks filter(const Landmarks &landmarks68, LandmarksType);

    template<typename... LandmarksTypes>
    static Landmarks filter(const Landmarks &landmarks68, LandmarksType type, LandmarksTypes... types)
    {
        auto landmarks0 = filter(landmarks68, type);
        auto landmarks1 = filter(landmarks68, types...);
        landmarks0.insert(landmarks0.end(), landmarks1.begin(), landmarks1.end());
        return landmarks0;
    }

private:
    bool loadData(const std::filesystem::path &cascadeDataPath, const std::filesystem::path &facemarkDataPath);

    std::vector<Rect> findBBoxes(const Mat &img);

    bool m_dataLoaded = false;
    CascadeClassifier m_faceDetector;
    Ptr<face::Facemark> m_facemark;
};

}    // namespace cv
