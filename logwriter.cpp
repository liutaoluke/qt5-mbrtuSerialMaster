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
    else {
        // QTextStream out(&logFile);
        // out << "Log file test write." << Qt::endl;
        qInfo("open log file successfully!!!");
    }

    qDebug() << "constructor - LogWriter::LogWriter - ThreadId: " << QThread::currentThreadId();
}

LogWriter::~LogWriter() {
    qCritical() << "destructor - LogWriter::~LogWriter()!!!";
    qDebug() << "destructor - LogWriter::~LogWriter - ThreadId: " << QThread::currentThreadId();

    mp_flushTimer->stop();
    flushLogs(); // 确保所有日志在析构时写入文件
    if (logFile.isOpen()) {
        logFile.close();
    }
}

void LogWriter::do_initFlushTimer() {
    qDebug() << "LogWriter::do_initFlushTimer - ThreadId: " << QThread::currentThreadId();

    mp_flushTimer = new QTimer(this);
    // 设置定时器，每600秒刷新一次日志
    mp_flushTimer->setInterval(600e3);
    auto connect_status = connect(mp_flushTimer, &QTimer::timeout, this, &LogWriter::flushLogs);
    qDebug() << "LogWriter::do_initFlushTimer - connect_status is " << connect_status;
    mp_flushTimer->start();
}

void LogWriter::writeLog(const QString &message) {
    // qDebug() << "LogWriter::writeLog - ThreadId: " << QThread::currentThreadId();
    QMutexLocker locker(&mutex);
    logQueue.enqueue(message);
}

void LogWriter::flushLogs() {
    // qDebug() << "LogWriter::flushLogs - ThreadId: " << QThread::currentThreadId();
    if (logQueue.isEmpty()) {
        qCritical() << "LogWriter::flushLogs - logQueue.isEmpty() - yes!!!";
        return;
    }
    // qInfo() << "LogWriter::flushLogs - logQueue.isEmpty() - no!!!";
    QMutexLocker locker(&mutex);
    QTextStream out(&logFile);
    while (!logQueue.isEmpty()) {
        out << logQueue.dequeue() << Qt::endl;
    }
}
