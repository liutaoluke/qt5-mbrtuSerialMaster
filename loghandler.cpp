#include "loghandler.h"
#include <QDateTime>
#include <QDebug>

LogHandler::LogHandler() {
    logWriter = new LogWriter("application.log");
    logThread = new QThread();
    logWriter->moveToThread(logThread);
    // connect(logThread, &QThread::started, logWriter, &LogWriter::do_startFlushTimer);
    // connect(logThread, &QThread::finished, logWriter, &LogWriter::do_stopFlushTimer);
    connect(logThread, &QThread::finished, logWriter, &QObject::deleteLater);
    connect(this, &LogHandler::sg_logMessage, logWriter, &LogWriter::writeLog);
    connect(this, &LogHandler::sg_flushLogs, logWriter, &LogWriter::flushLogs);
    logThread->start();
}

LogHandler::~LogHandler() {
    qCritical() << "destruction - LogHandler::~LogHandler()!!!";

    logWriter->do_stopFlushTimer();
    logThread->quit(); // 请求线程退出事件循环
    logThread->wait(); // 等待线程完成所有任务并退出
    delete logThread;  // 删除线程对象
}

void LogHandler::handleLogMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    qDebug() << "LogHandler::handleLogMessage - ThreadId: " << QThread::currentThreadId();
    QString logMessage;
    switch (type) {
    case QtDebugMsg:
        logMessage = QString("Debug: %1").arg(msg);
        qDebug().noquote() << logMessage;
        return;
        break;
    case QtInfoMsg:
        logMessage = QString("Info: %1").arg(msg);
        qInfo().noquote() << logMessage;
        return;
        break;
    case QtWarningMsg:
        logMessage = QString("Warning: %1").arg(msg);
        qWarning().noquote() << logMessage;
        return;
        break;
    case QtCriticalMsg:
        logMessage = QString("Critical: %1").arg(msg);
        qCritical().noquote() << logMessage;
        return;
        break;
    case QtFatalMsg:
        logMessage = QString("Fatal: %1").arg(msg);
        emit sg_flushLogs(); // 在发生致命错误时立即刷新日志
        abort();             // 确保程序终止
    default:
        qWarning() << "type of logMessage is not exsited";
        return;
    }

    logMessage = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ") + QString(context.file)
                 + ":" + QString::number(context.line) + ", " + context.function + " - " + logMessage;

    emit sg_logMessage(logMessage);
}
