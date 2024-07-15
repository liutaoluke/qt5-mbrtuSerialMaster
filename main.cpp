#include "mainwindow.h"

#include <QApplication>

#include <QDebug>
#include "ess.h"
#include "os.h"
#include "serialcommu.h"
#include "serialrequest.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

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

    return a.exec();
}
