#include "mainwindow.h"

#include <QApplication>

#include "meter.h"
#include "modbushandler.h"
#include "os.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    OS os;

    ModbusHandler handler(&os, "/dev/ttyS2");
    handler.start();

    Meter meterGrid(21);
    Meter meterLoad(22);
    Meter meterEss(23);

    QTimer timer;
    timer.setInterval(1e3);
    timer.start();
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        ++os.stat.qtimer.cnt_500ms;
        handler.sendRequest(meterGrid.m_mbRequestGetDataVAW);
        handler.sendRequest(meterLoad.m_mbRequestGetDataVAW);
        handler.sendRequest(meterEss.m_mbRequestGetDataVAW);
        qDebug() << " meterGrid.m_mbRequestGetDataVAW : " << meterGrid.m_mbDataUnitReplyGetDataVAW.values();
        qDebug() << " meterLoad.m_mbRequestGetDataVAW : " << meterLoad.m_mbDataUnitReplyGetDataVAW.values();
        qDebug() << "  meterEss.m_mbRequestGetDataVAW : " << meterEss.m_mbDataUnitReplyGetDataVAW.values();
    });

    QTimer timer2;
    timer2.setInterval(5e3);
    timer2.start();
    QObject::connect(&timer2, &QTimer::timeout, [&]() {
        ++os.stat.qtimer.cnt_05sec;
        handler.sendRequest(meterGrid.m_mbRequestGetDataEnergy);
        handler.sendRequest(meterLoad.m_mbRequestGetDataEnergy);
        handler.sendRequest(meterEss.m_mbRequestGetDataEnergy);
        qDebug() << " meterGrid.m_mbRequestGetDataEnergy : "
                 << meterGrid.m_mbDataUnitReplyGetDataEnergy.values();
        qDebug() << " meterLoad.m_mbRequestGetDataEnergy : "
                 << meterLoad.m_mbDataUnitReplyGetDataEnergy.values();
        qDebug() << "  meterEss.m_mbRequestGetDataEnergy : "
                 << meterEss.m_mbDataUnitReplyGetDataEnergy.values();
    });

    // QTimer timer3;
    // timer3.setInterval(30e3);
    // timer3.start();

    QTimer timer4;
    timer4.setInterval(120e3);
    timer4.start();
    QObject::connect(&timer4, &QTimer::timeout, [&]() {
        ++os.stat.qtimer.cnt_02min;
        handler.sendRequest(meterGrid.m_mbRequestGetParaSys);
        handler.sendRequest(meterLoad.m_mbRequestGetParaSys);
        handler.sendRequest(meterEss.m_mbRequestGetParaSys);
        qDebug() << " meterGrid.m_mbRequestGetParaSys : " << meterGrid.m_mbDataUnitReplyGetParaSys.values();
        qDebug() << " meterLoad.m_mbRequestGetParaSys : " << meterLoad.m_mbDataUnitReplyGetParaSys.values();
        qDebug() << "  meterEss.m_mbRequestGetParaSys : " << meterEss.m_mbDataUnitReplyGetParaSys.values();
    });

    QTimer timer5;
    timer5.setInterval(600e3);
    timer5.start();
    QObject::connect(&timer5, &QTimer::timeout, [&]() {
        ++os.stat.qtimer.cnt_10min;
        handler.sendRequest(meterGrid.m_mbRequestGetParaTOUA);
        handler.sendRequest(meterLoad.m_mbRequestGetParaTOUA);
        handler.sendRequest(meterEss.m_mbRequestGetParaTOUA);
        qDebug() << " meterGrid.m_mbRequestGetParaTOUA : " << meterGrid.m_mbDataUnitReplyGetParaTouA.values();
        qDebug() << " meterLoad.m_mbRequestGetParaTOUA : " << meterLoad.m_mbDataUnitReplyGetParaTouA.values();
        qDebug() << "  meterEss.m_mbRequestGetParaTOUA : " << meterEss.m_mbDataUnitReplyGetParaTouA.values();
    });

    MainWindow w(&timer, &os);
    w.show();
    return a.exec();
}
