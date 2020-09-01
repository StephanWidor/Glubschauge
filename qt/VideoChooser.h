#pragma once
#include <QObject>
#include <QString>

namespace qt {

class VideoChooser : public QObject
{
    Q_OBJECT
public:
    VideoChooser(QObject *pParent = nullptr): QObject(pParent) {}

    Q_INVOKABLE QString chooseVideo();

private:
#ifdef ANDROID
    QString contentUriInfo(const QString &url, const QString &columnName);
#endif
};

}    // namespace qt
