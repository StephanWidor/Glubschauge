#pragma once
#include <QtDebug>

#include <sstream>
#include <thread>

namespace qt {

class Logger
{
public:
    static QDebug debug() { return qDebug(); }
    static QDebug info() { return qInfo(); }
    static QDebug critical() { return qCritical(); }
};

}    // namespace qt

inline QDebug operator<<(QDebug log, const std::string &text)
{
    return log << QString::fromStdString(text);
}

inline QDebug operator<<(QDebug log, const std::thread::id &id)
{
    std::stringstream threadId;
    threadId << id;
    return log << std::stoul(threadId.str());
}
