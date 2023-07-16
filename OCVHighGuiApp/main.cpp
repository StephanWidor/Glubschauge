#include <cv/GlubschEffect.h>
#include <cv/OutputDevice.h>
#include <opencv2/opencv.hpp>
#include <optional>

int main(int, char *[])
{
    cv::namedWindow("Glubschauge", cv::WINDOW_AUTOSIZE);
    cv::VideoCapture cap;
    cv::Mat frame;
    bool run{true};
    cv::GlubschEffect glubschEffect("haarcascade_frontalface_alt2.xml", "lbfmodel.yaml");
    std::optional<cv::OutputDevice> outputStream;

    const auto printDistorts = [&]() {
        std::cout << "Distortions:\n";
        std::cout << "\tEyes: " << glubschEffect.getDistort(cv::FaceDistortionType::Eyes) << std::endl;
        std::cout << "\tNose: " << glubschEffect.getDistort(cv::FaceDistortionType::Nose) << std::endl;
        std::cout << "\tMouth: " << glubschEffect.getDistort(cv::FaceDistortionType::Mouth) << std::endl;
        std::cout << "\tUpper Head: " << glubschEffect.getDistort(cv::FaceDistortionType::UpperHead) << std::endl;
        std::cout << "\tLower Head: " << glubschEffect.getDistort(cv::FaceDistortionType::LowerHead) << std::endl;
    };

    const auto incrementDistort = [&](cv::FaceDistortionType type) {
        glubschEffect.increment(type);
        printDistorts();
    };
    const auto decrementDistort = [&](cv::FaceDistortionType type) {
        glubschEffect.decrement(type);
        printDistorts();
    };

    const auto toggleDistortAlways = [&glubschEffect]() {
        glubschEffect.setDistortAlways(!glubschEffect.getDistortAlways());
        std::cout << "Distort always: " << glubschEffect.getDistortAlways() << std::endl;
    };

    const auto toggleDrawLandmarks = [&glubschEffect]() {
        glubschEffect.setDrawLandmarks(!glubschEffect.getDrawLandmarks());
        std::cout << "Drawe Landmarks: " << glubschEffect.getDrawLandmarks() << std::endl;
    };

    const auto toggleOutputStream = [&outputStream]() {
        if (outputStream == std::nullopt)
        {
            const auto device = "/dev/video5";
            outputStream.emplace(device);
            std::cout << "Streaming to " << device << std::endl;
        }
        else
        {
            outputStream.reset();
            std::cout << "Streaming stopped" << std::endl;
        }
    };

    const auto handleKeyInput = [&](const auto key) {
        switch (key)
        {
            case 'a':
                toggleDistortAlways();
                break;
            case 'd':
                toggleDrawLandmarks();
                break;
            case 'e':
                decrementDistort(cv::FaceDistortionType::Eyes);
                break;
            case 'E':
                incrementDistort(cv::FaceDistortionType::Eyes);
                break;
            case 'n':
                decrementDistort(cv::FaceDistortionType::Nose);
                break;
            case 'N':
                incrementDistort(cv::FaceDistortionType::Nose);
                break;
            case 'm':
                decrementDistort(cv::FaceDistortionType::Mouth);
                break;
            case 'M':
                incrementDistort(cv::FaceDistortionType::Mouth);
                break;
            case 'u':
                decrementDistort(cv::FaceDistortionType::UpperHead);
                break;
            case 'U':
                incrementDistort(cv::FaceDistortionType::UpperHead);
                break;
            case 'l':
                decrementDistort(cv::FaceDistortionType::LowerHead);
                break;
            case 'L':
                incrementDistort(cv::FaceDistortionType::LowerHead);
                break;
            case 's':
                toggleOutputStream();
                break;
            case 'c':
                run = false;
                break;
            case 'q':
                run = false;
                break;
            default:
                break;
        }
    };

    cap.open(0, cv::CAP_ANY);
    if (!cap.isOpened())
    {
        std::cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    while (run)
    {
        if (cap.read(frame))
        {
            glubschEffect.process(frame);
            cv::imshow("Glubschauge", frame);
            if (outputStream)
                outputStream->push(frame);
        }
        handleKeyInput(cv::waitKey(5));
    }
    cv::destroyAllWindows();
    return 0;
}
