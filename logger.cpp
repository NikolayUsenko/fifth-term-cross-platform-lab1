#include "logger.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>

Logger::Logger(QObject *parent)
    : QObject(parent)
    , m_fileName("log.txt")
{
}

void Logger::logResult(double result)
{
    QFile file(m_fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        emit logWritten("Error: cannot open log file");
        return;
    }

    QTextStream out(&file);
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
        << " Result: " << result << "\n";

    file.close();

    emit logWritten("Result logged");
}

void Logger::logError(const QString &message)
{
    QFile file(m_fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        emit logWritten("Error: cannot open log file");
        return;
    }

    QTextStream out(&file);
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
        << " Error: " << message << "\n";

    file.close();

    emit logWritten("Error logged");
}