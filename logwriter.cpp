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
    // 设置定时器，每10秒刷新一次日志
    mp_flushTimer->setInterval(10e3);
    connect(mp_flushTimer, &QTimer::timeout, this, &LogWriter::flushLogs);
    mp_flushTimer->start();
}

void LogWriter::writeLog(const QString &message) {
    qDebug() << "LogWriter::writeLog - ThreadId: " << QThread::currentThreadId();
    QMutexLocker locker(&mutex);
    logQueue.enqueue(message);
}

void LogWriter::flushLogs() {
    qDebug() << "LogWriter::flushLogs - ThreadId: " << QThread::currentThreadId();
    QMutexLocker locker(&mutex);
    QTextStream out(&logFile);
    while (!logQueue.isEmpty()) {
        out << logQueue.dequeue() << Qt::endl;
    }
}
