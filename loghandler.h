#pragma once
#include "def.h"

#include <QObject>
#include <QThread>
#include "logwriter.h"

class LogHandler : public QObject {
    Q_OBJECT

public:
    LogHandler();
    ~LogHandler();

    void handleLogMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

signals:
    void sg_logMessage(const QString &message);
    void sg_flushLogs();

private:
    LogWriter *logWriter;
    QThread *logThread;
};
