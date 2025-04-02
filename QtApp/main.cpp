#include "qt/ProcessingFilter.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <logger.h>

#if QT_CONFIG(permissions)
#include <QPermission>
#endif

int main(int argc, char *argv[])
{
    QApplication qApplication(argc, argv);
    qt::ProcessingFilter processingFilter;
    QQmlApplicationEngine m_qmlEngine;
    qmlRegisterUncreatableType<qt::ProcessingFilter>("stephanwidor.Glubschauge", 1, 0, "ProcessingFilter",
                                                     "Don't instantiate!");
    m_qmlEngine.rootContext()->setContextProperty("processingFilter", &processingFilter);

#if QT_CONFIG(permissions)
    QCameraPermission cameraPermission;
    qApplication.requestPermission(cameraPermission, [&](const QPermission &permission) {
        if (permission.status() != Qt::PermissionStatus::Granted)
            logger::out << "Didn't get Camera Permission";
        m_qmlEngine.load(":/qml/main.qml");
    });
#else
    m_qmlEngine.load(":/qml/main.qml");
#endif

    return qApplication.exec();
}
