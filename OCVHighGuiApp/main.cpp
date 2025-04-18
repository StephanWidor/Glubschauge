#include "resource_help.h"
#include <cctype>
#include <cv/GlubschEffect.h>
#include <cv/V4lLoopbackDevice.h>
#include <filesystem>

#include <fstream>
#include <opencv2/opencv.hpp>
#include <optional>

int main(int, char *[])
{
    const auto appDataDir = []() {
        if (const auto home = std::getenv("HOME"))
        {
            auto dir = std::filesystem::path{home} / ".config/Glubschauge";
            if (std::filesystem::exists(dir) || std::filesystem::create_directories(dir))
                return dir;
        }
        return std::filesystem::temp_directory_path();
    }();

    const auto saveModelFileToAppDataDir = [&](const std::string_view fileName, const std::string &content) {
        auto path = appDataDir / fileName;
        if (!std::filesystem::exists(path))
        {
            std::ofstream out(path.string());
            if (out.is_open())
                out << content;
        }
        return path;
    };

    const auto configPath = appDataDir / "GlubschConfig.yaml";
    const auto haarcascadePath = saveModelFileToAppDataDir("haarcascade_frontalface_default.xml", haarcascade_xml);
    const auto lbfmodelPath = saveModelFileToAppDataDir("lbfmodel.yaml", lbfmodel_yaml);

    cv::namedWindow("Glubschauge", cv::WINDOW_AUTOSIZE);
    cv::VideoCapture cap;
    cv::Mat frame;
    bool run{true};
    std::optional<cv::V4lLoopbackDevice> outputStream;

    cv::GlubschEffect glubschEffect(haarcascadePath, lbfmodelPath, cv::loadGlubschConfigFromYaml(configPath));
    auto &config = glubschEffect.config;

    cv::FaceDistortionType distortionTypeToAdapt = cv::FaceDistortionType::Num;

    const auto setDistortionToAdapt = [&](const cv::FaceDistortionType type) {
        distortionTypeToAdapt = type;
        std::cout << std::format("Use up and down arrows to adapt {} distortion", cv::name(type)) << std::endl;
    };

    const auto incrementDistort = [&](cv::FaceDistortionType type) {
        if (type < cv::FaceDistortionType::Num)
        {
            std::cout << std::format("{} distortion set to {}", cv::name(type), cv::increment(config.distortions, type))
                      << std::endl;
        }
    };
    const auto decrementDistort = [&](cv::FaceDistortionType type) {
        if (type < cv::FaceDistortionType::Num)
        {
            std::cout << std::format("{} distortion set to {}", cv::name(type), cv::decrement(config.distortions, type))
                      << std::endl;
        }
    };

    const auto toggleDistortAlways = [&]() {
        config.distortAlways = !config.distortAlways;
        std::cout << std::format("Distort always: {}", config.distortAlways) << std::endl;
    };

    const auto toggleDrawLandmarks = [&]() {
        config.drawLandmarks = !config.drawLandmarks;
        std::cout << "Draw Landmarks: " << config.drawLandmarks << std::endl;
    };

    const auto toggleOutputStream = [&outputStream]() {
        const auto device = "/dev/video5";
        if (outputStream == std::nullopt)
        {
            outputStream.emplace(device);
            if (outputStream->ok())
                std::cout << std::format("Streaming to {}", device) << std::endl;
            else
            {
                outputStream.reset();
                std::cout << std::format("Cannot stream to {}", device) << std::endl;
                std::cout << "Have you inserted a v4l2loopback device? If not, try" << std::endl;
                std::cout << "\tsudo modprobe v4l2loopback exclusive_caps=1 video_nr=5 card_label=\"Glubschauge Cam\""
                          << std::endl;
            }
        }
        else
        {
            outputStream.reset();
            std::cout << std::format("Stopped streaming to {}", device) << std::endl;
        }
    };

#ifdef WIN32
    constexpr auto downKey = 2621440;
    constexpr auto upKey = 2490368;
#else
    constexpr auto downKey = 65364;
    constexpr auto upKey = 65362;
#endif

    const auto handleKeyInput = [&](const auto key) {
        switch (key)
        {
            case downKey:
            {
                decrementDistort(distortionTypeToAdapt);
                return;
            }
            case upKey:
            {
                incrementDistort(distortionTypeToAdapt);
                return;
            }
            default:
                break;
        }
        switch (std::tolower(key))
        {
            case 'a':
                toggleDistortAlways();
                break;
            case 'd':
                toggleDrawLandmarks();
                break;
            case 'e':
                setDistortionToAdapt(cv::FaceDistortionType::Eyes);
                break;
            case 'n':
                setDistortionToAdapt(cv::FaceDistortionType::Nose);
                break;
            case 'm':
                setDistortionToAdapt(cv::FaceDistortionType::Mouth);
                break;
            case 'u':
                setDistortionToAdapt(cv::FaceDistortionType::UpperHead);
                break;
            case 'l':
                setDistortionToAdapt(cv::FaceDistortionType::LowerHead);
                break;
            case 's':
                if (cv::V4lLoopbackDevice::implemented())
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
            if (outputStream)
                outputStream->push(frame);
            cv::flip(frame, frame, 1);
            cv::imshow("Glubschauge", frame);
        }
        handleKeyInput(cv::waitKeyEx(5));
    }
    cv::destroyAllWindows();
    cv::saveToYaml(glubschEffect.config, configPath);
    return 0;
}
