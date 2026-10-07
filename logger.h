#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>

class Logger : public QObject {
    Q_OBJECT

public:
    explicit Logger(QObject *parent = nullptr);

public slots:
    void logResult(double result);
    void logError(const QString &message);

signals:
    void logWritten(const QString &message);

private:
    QString m_fileName;
};

#endif // LOGGER_H
