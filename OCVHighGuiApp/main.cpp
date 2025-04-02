#include <cv/GlubschEffect.h>
#include <cv/V4lLoopbackDevice.h>
#include <opencv2/opencv.hpp>
#include <optional>

int main(int, char *[])
{
    cv::namedWindow("Glubschauge", cv::WINDOW_AUTOSIZE);
    cv::VideoCapture cap;
    cv::Mat frame;
    bool run{true};

    const auto configPath = []() {
        if (const auto home = std::getenv("HOME"))
            return std::filesystem::path{home} / ".local/share/Glubschauge/GlubschConfig.yaml";
        return std::filesystem::temp_directory_path() / "GlubschConfig.yaml";
    }();

    cv::GlubschEffect glubschEffect("haarcascade_frontalface_alt2.xml", "lbfmodel.yaml",
                                    cv::loadGlubschConfigFromYaml(configPath));
    auto &config = glubschEffect.config;
    std::optional<cv::V4lLoopbackDevice> outputStream;

    const auto getDistort = [&](const cv::FaceDistortionType type) -> double {
        return cv::get(config.distortions, type);
    };

    const auto printDistortions = [&]() {
        std::cout << "Distortions:\n";
        std::cout << "\tEyes: " << getDistort(cv::FaceDistortionType::Eyes) << std::endl;
        std::cout << "\tNose: " << getDistort(cv::FaceDistortionType::Nose) << std::endl;
        std::cout << "\tMouth: " << getDistort(cv::FaceDistortionType::Mouth) << std::endl;
        std::cout << "\tUpper Head: " << getDistort(cv::FaceDistortionType::UpperHead) << std::endl;
        std::cout << "\tLower Head: " << getDistort(cv::FaceDistortionType::LowerHead) << std::endl;
    };

    const auto incrementDistort = [&](cv::FaceDistortionType type) {
        cv::increment(config.distortions, type);
        printDistortions();
    };
    const auto decrementDistort = [&](cv::FaceDistortionType type) {
        cv::decrement(config.distortions, type);
        printDistortions();
    };

    const auto toggleDistortAlways = [&]() {
        config.distortAlways = !config.distortAlways;
        std::cout << "Distort always: " << config.distortAlways << std::endl;
    };

    const auto toggleDrawLandmarks = [&]() {
        config.drawLandmarks = !config.drawLandmarks;
        std::cout << "Drawe Landmarks: " << config.drawLandmarks << std::endl;
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
    cv::saveToYaml(glubschEffect.config, configPath);
    return 0;
}
