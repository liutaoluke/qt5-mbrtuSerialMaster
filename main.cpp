#include "mainwindow.h"

#include <QApplication>

#include "modbushandler.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    ModbusHandler handler("/dev/ttyS2");
    handler.start();

    QModbusDataUnit mbDataUnitMeterData01(QModbusDataUnit::HoldingRegisters, 0x2000, 82);
    QModbusDataUnit mbDataUnitMeterData02(QModbusDataUnit::HoldingRegisters, 0x101E, 60);
    QModbusDataUnit mbDataUnitMeterPara01(QModbusDataUnit::HoldingRegisters, 0x0000, 53);
    QModbusDataUnit mbDataUnitMeterPara02(QModbusDataUnit::HoldingRegisters, 0x6000, 56);

    QVector<quint16> meter21Data01;
    QVector<quint16> meter21Data02;
    QVector<quint16> meter21Para01;
    QVector<quint16> meter21Para02;
    ModbusRequest requestMeter21Data01(mbDataUnitMeterData01, 21, &meter21Data01);
    ModbusRequest requestMeter21Data02(mbDataUnitMeterData02, 21, &meter21Data02);
    ModbusRequest requestMeter21Para01(mbDataUnitMeterPara01, 21, &meter21Para01);
    ModbusRequest requestMeter21Para02(mbDataUnitMeterPara02, 21, &meter21Para02);

    QVector<quint16> meter22Data01;
    QVector<quint16> meter22Data02;
    QVector<quint16> meter22Para01;
    QVector<quint16> meter22Para02;
    ModbusRequest requestMeter22Data01(mbDataUnitMeterData01, 22, &meter22Data01);
    ModbusRequest requestMeter22Data02(mbDataUnitMeterData02, 22, &meter22Data02);
    ModbusRequest requestMeter22Para01(mbDataUnitMeterPara01, 22, &meter22Para01);
    ModbusRequest requestMeter22Para02(mbDataUnitMeterPara02, 22, &meter22Para02);

    QTimer timer;
    timer.setInterval(1000);
    timer.start();
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        handler.sendRequest(requestMeter21Data01);
        handler.sendRequest(requestMeter21Data02);
        handler.sendRequest(requestMeter21Para01);
        handler.sendRequest(requestMeter21Para02);
        qDebug() << " QVector<quint16> meter21Data01 : " << meter21Data01;
        qDebug() << " QVector<quint16> meter21Data02 : " << meter21Data02;
        qDebug() << " QVector<quint16> meter21Para01 : " << meter21Para01;
        qDebug() << " QVector<quint16> meter21Para02 : " << meter21Para02;
    });

    QTimer timer2;
    timer2.setInterval(2000);
    timer2.start();
    QObject::connect(&timer2, &QTimer::timeout, [&]() {
        handler.sendRequest(requestMeter22Data01);
        handler.sendRequest(requestMeter22Data02);
        handler.sendRequest(requestMeter22Para01);
        handler.sendRequest(requestMeter22Para02);
        qDebug() << " QVector<quint16> meter22Data01 : " << meter22Data01;
        qDebug() << " QVector<quint16> meter22Data02 : " << meter22Data02;
        qDebug() << " QVector<quint16> meter22Para01 : " << meter22Para01;
        qDebug() << " QVector<quint16> meter22Para02 : " << meter22Para02;
    });

    MainWindow w;
    w.show();
    return a.exec();
}
