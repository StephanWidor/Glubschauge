#include "cv/FaceDetection.h"
#include "cv/ImageUtils.h"
#include "logger.h"

namespace cv {

FaceDetection::FaceDetection(const std::string &cascadeDataPath, const std::string &facemarkDataPath)
    : m_dataLoaded(false), m_facemark(face::FacemarkLBF::create())
{
    m_dataLoaded = loadData(cascadeDataPath, facemarkDataPath);
}

bool FaceDetection::loadData(const std::string &cascadeDataPath, const std::string &facemarkDataPath)
{
    try
    {
        if (m_faceDetector.load(cascadeDataPath))
        {
            m_facemark->loadModel(facemarkDataPath);
            return true;
        }
    }
    catch (...)
    {}
    logger::out << "couldn't load all data!";
    return false;
}

std::pair<FaceDetection::BBoxes, std::vector<FaceDetection::Landmarks>> FaceDetection::detect(const Mat &img)
{
    if (m_dataLoaded)
    {
        try
        {
            // TODO: investigate why this can throw
            BBoxes bboxes = findBBoxes(img);
            std::vector<Landmarks> landmarks;
            m_facemark->fit(img, bboxes, landmarks);
            return {bboxes, landmarks};
        }
        catch (cv::Exception &e)
        {
            // TODO: find out why this happens sometimes
            logger::out << "caught exception from OpenCV:";
            logger::out << e.what();
        }
    }
    return {};
}

std::vector<Rect> FaceDetection::findBBoxes(const Mat &img)
{
    cv::Mat processImg;
    const Size fittingSize(240, 240);
    const auto scaleFactor = ImageUtils::fitToSize(img, fittingSize, true, processImg);
    if (scaleFactor == HUGE_VAL)
        return {};
    if (processImg.channels() == 3)
        cvtColor(processImg, processImg, COLOR_BGR2GRAY);
    equalizeHist(processImg, processImg);
    std::vector<Rect> bboxes;
    m_faceDetector.detectMultiScale(processImg, bboxes);
    if (scaleFactor != 1.0)
    {
        const auto backScaleFactor = 1.0 / scaleFactor;
        for (auto &box : bboxes)
            box = Utils2D::scale(box, backScaleFactor);
    }
    return bboxes;
}

FaceDetection::Landmarks FaceDetection::filter(const Landmarks &landmarks68, LandmarksType type)
{
    assert(landmarks68.size() == 68);
    switch (type)
    {
        case LandmarksType::JawLine:
            return Landmarks(landmarks68.begin(), landmarks68.begin() + 17);
        case LandmarksType::LeftEyebrow:
            return Landmarks(landmarks68.begin() + 17, landmarks68.begin() + 22);
        case LandmarksType::RightEyebrow:
            return Landmarks(landmarks68.begin() + 22, landmarks68.begin() + 27);
        case LandmarksType::NoseBridge:
            return Landmarks(landmarks68.begin() + 27, landmarks68.begin() + 31);
        case LandmarksType::LowerNose:
            return Landmarks(landmarks68.begin() + 31, landmarks68.begin() + 36);
        case LandmarksType::LeftEye:
            return Landmarks(landmarks68.begin() + 36, landmarks68.begin() + 42);
        case LandmarksType::RightEye:
            return Landmarks(landmarks68.begin() + 42, landmarks68.begin() + 48);
        case LandmarksType::OuterLip:
            return Landmarks(landmarks68.begin() + 48, landmarks68.begin() + 60);
        case LandmarksType::InnerLip:
            return Landmarks(landmarks68.begin() + 60, landmarks68.begin() + 68);
    };
    return {};
}

}    // namespace cv
