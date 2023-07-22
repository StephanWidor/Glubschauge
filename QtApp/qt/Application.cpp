#include "qt/Application.h"
#include "qt/VideoChooser.h"

#include <QQmlContext>

qt::Application::Application(int argc, char *argv[]): m_qApplication(argc, argv)
{
    // FileDialog complains without this, but we set organization to empty because appdata location changes otherwise
    m_qApplication.setOrganizationName("");
    m_qApplication.setOrganizationDomain("glubsch.widor.site");
    qmlRegisterUncreatableType<qt::ProcessingFilter>("stephanwidor.Glubschauge", 1, 0, "ProcessingFilter",
                                                     "Don't instantiate!");
    m_qmlEngine.rootContext()->setContextProperty("processingFilter", &m_processingFilter);
    qmlRegisterSingletonType<qt::VideoChooser>(
      "stephanwidor.Glubschauge", 1, 0, "VideoChooser",
      [](QQmlEngine *, QJSEngine *) -> QObject * { return new qt::VideoChooser(); });
    m_qmlEngine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));
}
