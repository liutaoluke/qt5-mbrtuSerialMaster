#include "mainwindow.h"

#include <QApplication>

#include <QDebug>
#include "ess.h"
#include "os.h"
#include "serialcommu.h"
#include "serialrequest.h"

#include "loghandler.h"

LogHandler *globalLogHandler = nullptr;
// 自定义消息处理函数
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    if (globalLogHandler) {
        globalLogHandler->handleLogMessage(type, context, msg);
    }
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 注册自定义消息处理函数
    globalLogHandler = new LogHandler();
    qInstallMessageHandler(customMessageHandler);

    OS os[3];
    ESS ess(os);

    MainWindow w(os);
    w.show();

    SerialCommu serialCommuCom3;

    SerialRequest serialRequestData01("hello luke");
    SerialRequest serialRequestData02("hello world");
    SerialRequest serialRequestData03("hello jiajia");

    bool isEnable_request_queue = true;

    QTimer timer;
    timer.start(1e3);
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        auto request_queue_size = serialCommuCom3.m_requestQueue.size();

        qDebug() << "requestQueue.size() : " << request_queue_size;

        if ((request_queue_size < 100) && (isEnable_request_queue)) {
            serialCommuCom3.sendRequest(serialRequestData01);
            serialCommuCom3.sendRequest(serialRequestData02);
            serialCommuCom3.sendRequest(serialRequestData03);
        }
        else if (request_queue_size >= 100) {
            isEnable_request_queue = false;
        }
        else if (request_queue_size < 10) {
            isEnable_request_queue = true;
        }
    });

    // When application quits, stop the thread
    QObject::connect(&a, &QCoreApplication::aboutToQuit, [&]() {
        globalLogHandler->deleteLater();
        // delete globalLogHandler;
    });

    return a.exec();
}
