#include "logwriter.h"
#include <QDebug>
#include <QTextStream>
#include <QThread>

LogWriter::LogWriter(const QString &fileName, QObject *parent)
    : QObject(parent)
    , logFile(fileName) {
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        qCritical("Failed to open log file.");
    }

    qDebug() << "constructor - LogWriter::LogWriter - ThreadId: " << QThread::currentThreadId();
    // 设置定时器，每60秒刷新一次日志
    flushTimer.setInterval(60e3);
    connect(&flushTimer, &QTimer::timeout, this, &LogWriter::flushLogs);
    flushTimer.start();
}

LogWriter::~LogWriter() {
    qCritical() << "destructor - LogWriter::~LogWriter()!!!";
    qDebug() << "destructor - LogWriter::~LogWriter - ThreadId: " << QThread::currentThreadId();

    flushTimer.stop();
    flushLogs(); // 确保所有日志在析构时写入文件
    if (logFile.isOpen()) {
        logFile.close();
    }
}

void LogWriter::do_startFlushTimer() {
    qDebug() << "LogWriter::do_startFlushTimer - ThreadId: " << QThread::currentThreadId();
    flushTimer.start();
}

void LogWriter::do_stopFlushTimer() {
    qDebug() << "LogWriter::do_stopFlushTimer - ThreadId: " << QThread::currentThreadId();
    flushTimer.stop();
}

void LogWriter::writeLog(const QString &message) {
    qDebug() << "LogWriter::writeLog - ThreadId: " << QThread::currentThreadId();
    // QMutexLocker locker(&mutex);
    logQueue.enqueue(message);
}

void LogWriter::flushLogs() {
    qDebug() << "LogWriter::flushLogs - ThreadId: " << QThread::currentThreadId();
    // QMutexLocker locker(&mutex);
    QTextStream out(&logFile);
    while (!logQueue.isEmpty()) {
        out << logQueue.dequeue() << Qt::endl;
    }
}
