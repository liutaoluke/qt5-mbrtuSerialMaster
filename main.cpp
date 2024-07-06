#include "mainwindow.h"

#include <QApplication>

#include "modbushandler.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    ModbusHandler handler("/dev/ttyS2");
    handler.start();

    const QModbusDataUnit mbDataUnitMeterData01(QModbusDataUnit::HoldingRegisters, 0x2000, 82);
    const QModbusDataUnit mbDataUnitMeterData02(QModbusDataUnit::HoldingRegisters, 0x101E, 60);
    const QModbusDataUnit mbDataUnitMeterPara01(QModbusDataUnit::HoldingRegisters, 0x0000, 53);
    const QModbusDataUnit mbDataUnitMeterPara02(QModbusDataUnit::HoldingRegisters, 0x6000, 56);

    QModbusDataUnit mbDataUnitReplyMeter21Data01;
    QModbusDataUnit mbDataUnitReplyMeter21Data02;
    QModbusDataUnit mbDataUnitReplyMeter21Para01;
    QModbusDataUnit mbDataUnitReplyMeter21Para02;
    ModbusRequest requestMeter21Data01(mbDataUnitMeterData01, 21, &mbDataUnitReplyMeter21Data01);
    ModbusRequest requestMeter21Data02(mbDataUnitMeterData02, 21, &mbDataUnitReplyMeter21Data02);
    ModbusRequest requestMeter21Para01(mbDataUnitMeterPara01, 21, &mbDataUnitReplyMeter21Para01);
    ModbusRequest requestMeter21Para02(mbDataUnitMeterPara02, 21, &mbDataUnitReplyMeter21Para02);

    QModbusDataUnit mbDataUnitReplyMeter22Data01;
    QModbusDataUnit mbDataUnitReplyMeter22Data02;
    QModbusDataUnit mbDataUnitReplyMeter22Para01;
    QModbusDataUnit mbDataUnitReplyMeter22Para02;
    ModbusRequest requestMeter22Data01(mbDataUnitMeterData01, 22, &mbDataUnitReplyMeter22Data01);
    ModbusRequest requestMeter22Data02(mbDataUnitMeterData02, 22, &mbDataUnitReplyMeter22Data02);
    ModbusRequest requestMeter22Para01(mbDataUnitMeterPara01, 22, &mbDataUnitReplyMeter22Para01);
    ModbusRequest requestMeter22Para02(mbDataUnitMeterPara02, 22, &mbDataUnitReplyMeter22Para02);

    QTimer timer;
    timer.setInterval(1000);
    timer.start();
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        handler.sendRequest(requestMeter21Data01);
        handler.sendRequest(requestMeter21Data02);
        handler.sendRequest(requestMeter21Para01);
        handler.sendRequest(requestMeter21Para02);
        qDebug() << " QVector<quint16> meter21Data01 : " << mbDataUnitReplyMeter21Data01.values();
        qDebug() << " QVector<quint16> meter21Data02 : " << mbDataUnitReplyMeter21Data02.values();
        qDebug() << " QVector<quint16> meter21Para01 : " << mbDataUnitReplyMeter21Para01.values();
        qDebug() << " QVector<quint16> meter21Para02 : " << mbDataUnitReplyMeter21Data02.values();
    });

    QTimer timer2;
    timer2.setInterval(2000);
    timer2.start();
    QObject::connect(&timer2, &QTimer::timeout, [&]() {
        handler.sendRequest(requestMeter22Data01);
        handler.sendRequest(requestMeter22Data02);
        handler.sendRequest(requestMeter22Para01);
        handler.sendRequest(requestMeter22Para02);
        qDebug() << " QVector<quint16> meter22Data01 : " << mbDataUnitReplyMeter22Data01.values();
        qDebug() << " QVector<quint16> meter22Data02 : " << mbDataUnitReplyMeter22Data02.values();
        qDebug() << " QVector<quint16> meter22Para01 : " << mbDataUnitReplyMeter22Para01.values();
        qDebug() << " QVector<quint16> meter22Para02 : " << mbDataUnitReplyMeter22Data02.values();
    });

    MainWindow w;
    w.show();
    return a.exec();
}
