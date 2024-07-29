#include "loghandler.h"
#include <QDateTime>
#include <QDebug>

LogHandler::LogHandler() {
    qDebug() << "constructor - LogHandler::LogHandler() - ThreadId: " << QThread::currentThreadId();

    mp_logWriter = new LogWriter("/root/application.log");
    mp_logThread = new QThread(this);
    mp_logWriter->moveToThread(mp_logThread);
    qDebug() << "LogWriter - ThreadId before thread start: "
             << mp_logWriter->thread()->currentThreadId();
#if 1
    bool connect_status = connect(mp_logThread,
                                  &QThread::started,
                                  mp_logWriter,
                                  &LogWriter::do_initFlushTimer);
    qDebug() << "mp_logThread & mp_logWriter - connect_status : " << connect_status;
#else
    mp_logWriter->do_initFlushTimer();
#endif
    connect(mp_logThread, &QThread::finished, mp_logWriter, &QObject::deleteLater);
    connect(this, &LogHandler::sg_logMessage, mp_logWriter, &LogWriter::writeLog);
    connect(this, &LogHandler::sg_flushLogs, mp_logWriter, &LogWriter::flushLogs);
    mp_logThread->start();
    bool isRunning_status = mp_logThread->isRunning();
    qDebug() << "mp_logThread - isRunning_status : " << isRunning_status;
    qDebug() << "LogWriter - ThreadId after thread start: "
             << mp_logWriter->thread()->currentThreadId();
}

LogHandler::~LogHandler() {
    qCritical() << "destructor - LogHandler::~LogHandler()!!!";
    qDebug() << "destructor - LogHandler::~LogHandler() - ThreadId: " << QThread::currentThreadId();

    deleteLogThread();
    globalLogHandler = nullptr;
    qInstallMessageHandler(nullptr);
}

void LogHandler::deleteLogThread() {
    if (mp_logThread->isRunning()) {
        mp_logThread->quit(); // 请求线程退出事件循环
        mp_logThread->wait(); // 等待线程完成所有任务并退出
    }
    mp_logWriter->deleteLater();
    mp_logThread->deleteLater(); // 删除线程对象
}

void LogHandler::handleLogMessage(QtMsgType type,
                                  const QMessageLogContext &context,
                                  const QString &msg) {
    // qDebug() << "LogHandler::handleLogMessage - ThreadId: " << QThread::currentThreadId();
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
        // return;
        break;
    case QtCriticalMsg:
        logMessage = QString("Critical: %1").arg(msg);
        qCritical().noquote() << logMessage;
        // return;
        break;
    case QtFatalMsg:
        logMessage = QString("Fatal: %1").arg(msg);
        emit sg_flushLogs(); // 在发生致命错误时立即刷新日志
        abort();             // 确保程序终止
    default:
        qDebug() << "type of logMessage is not exsited";
        return;
    }

    logMessage = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ")
                 + QString(context.file) + ":" + QString::number(context.line) + ", "
                 + context.function + " - " + logMessage;
    // qDebug() << "LogHandler::handleLogMessage - logMessage : " << logMessage;
    emit sg_logMessage(logMessage);
}
