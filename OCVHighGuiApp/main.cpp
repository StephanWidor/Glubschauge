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

    const auto toggleDistort = [&glubschEffect](cv::FaceDistortionType type) {
        glubschEffect.setDoDistort(type, !glubschEffect.getDoDistort(type));
        std::cout << "Distortions:\n";
        std::cout << "\tEyes: " << glubschEffect.getDoDistort(cv::FaceDistortionType::Eyes) << std::endl;
        std::cout << "\tNose: " << glubschEffect.getDoDistort(cv::FaceDistortionType::Nose) << std::endl;
        std::cout << "\tMouth: " << glubschEffect.getDoDistort(cv::FaceDistortionType::Mouth) << std::endl;
        std::cout << "\tUpper Head: " << glubschEffect.getDoDistort(cv::FaceDistortionType::UpperHead) << std::endl;
        std::cout << "\tLower Head: " << glubschEffect.getDoDistort(cv::FaceDistortionType::LowerHead) << std::endl;
    };

    const auto toggleDistortAlways = [&glubschEffect]() {
        glubschEffect.setDistortAlways(!glubschEffect.getDistortAlways());
        std::cout << "Distort always: " << glubschEffect.getDistortAlways() << std::endl;
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
            case 'e':
                toggleDistort(cv::FaceDistortionType::Eyes);
                break;
            case 'n':
                toggleDistort(cv::FaceDistortionType::Nose);
                break;
            case 'm':
                toggleDistort(cv::FaceDistortionType::Mouth);
                break;
            case 'u':
                toggleDistort(cv::FaceDistortionType::UpperHead);
                break;
            case 'l':
                toggleDistort(cv::FaceDistortionType::LowerHead);
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
