#include "mainwindow.h"

#include <QApplication>

#include <QDebug>
#include "ess.h"
#include "loghandler.h"

#include "serialcommu.h"
#include "serialrequest.h"

//globle definition
LogHandler *globalLogHandler = nullptr;
// 自定义消息处理函数
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    if (globalLogHandler) {
        globalLogHandler->handleLogMessage(type, context, msg);
    }
}

/***
    os[0]
    os[1] - com1
    os[2] - com2
    os[3] - com3
    os[4] - com4
    ***/
OS os[5]; //globle variable

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    qDebug() << "main() - ThreadId: " << QThread::currentThreadId();
    // 注册自定义消息处理函数
    globalLogHandler = new LogHandler();
    qInstallMessageHandler(customMessageHandler);

    // ESS ess;
    // QThread threadESS;
    // ess.moveToThread(&threadESS);
    // QObject::connect(&threadESS, &QThread::started, &ess, &ESS::do_initESSInSubThread);
    // threadESS.start();

    ESS *p_ess = new ESS;
    QThread *p_threadESS = new QThread;
    p_ess->moveToThread(p_threadESS);
    QObject::connect(p_threadESS, &QThread::started, p_ess, &ESS::do_initESSInSubThread);
    QObject::connect(p_threadESS, &QThread::finished, p_ess, &ESS::deleteLater);
    p_threadESS->start();

    MainWindow w;
    w.show();

    // When application quits, stop the thread
    QObject::connect(&a, &QCoreApplication::aboutToQuit, [&]() {
        if (p_threadESS->isRunning()) {
            p_threadESS->quit();
            p_threadESS->wait();
        }
        // delete p_ess;
        p_ess->deleteLater();
        // delete p_threadESS;
        p_threadESS->deleteLater();

        // delete globalLogHandler;
        globalLogHandler->deleteLater();
    });

    int execResult = a.exec();

    qInfo() << "Application execution finished!!!";

    return execResult;
}
