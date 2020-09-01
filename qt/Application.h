#pragma once
#include "qt/ProcessingFilter.h"
#include <QApplication>
#include <QQmlApplicationEngine>

namespace qt {

class Application
{
public:
    Application(int argc, char *argv[]);

    int run() { return m_qApplication.exec(); }

private:
    struct AttributeSettings
    {
        AttributeSettings() { QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling); }
    };

    AttributeSettings m_attributeSettings;
    QApplication m_qApplication;
    ProcessingFilter m_processingFilter;
    QQmlApplicationEngine m_qmlEngine;
};

}    // namespace qt
