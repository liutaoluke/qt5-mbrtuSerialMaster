#pragma once

#include <QFile>
#include <QMutex>
#include <QObject>
#include <QQueue>
#include <QTimer>

class LogWriter : public QObject {
    Q_OBJECT

public:
    explicit LogWriter(const QString &fileName, QObject *parent = nullptr);
    ~LogWriter();

public slots:
    void writeLog(const QString &message);
    void flushLogs();

private:
    QFile logFile;
    QMutex mutex;
    QQueue<QString> logQueue;
    QTimer flushTimer;
};
