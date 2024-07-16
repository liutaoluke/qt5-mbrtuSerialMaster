#include "logwriter.h"
#include <QTextStream>

LogWriter::LogWriter(const QString &fileName, QObject *parent)
    : QObject(parent)
    , logFile(fileName) {
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        qCritical("Failed to open log file.");
    }

    // 设置定时器，每10秒刷新一次日志
    flushTimer.setInterval(10e3);
    connect(&flushTimer, &QTimer::timeout, this, &LogWriter::flushLogs);
    flushTimer.start();
}

LogWriter::~LogWriter() {
    flushTimer.stop();
    flushLogs(); // 确保所有日志在析构时写入文件
    if (logFile.isOpen()) {
        logFile.close();
    }
}

void LogWriter::writeLog(const QString &message) {
    QMutexLocker locker(&mutex);
    logQueue.enqueue(message);
}

void LogWriter::flushLogs() {
    QMutexLocker locker(&mutex);
    QTextStream out(&logFile);
    while (!logQueue.isEmpty()) {
        out << logQueue.dequeue() << endl;
    }
}
