#include "serialcommu.h"

#include <QDebug>

SerialCommu::SerialCommu(QObject *parent)
    : QObject{parent}
    , m_requestInProgress(false) {

    mp_serialPort = new QSerialPort(this);

    mp_serialPort->setPortName("ttyS4");
    mp_serialPort->setBaudRate(QSerialPort::Baud9600);
    mp_serialPort->setDataBits(QSerialPort::Data8);
    mp_serialPort->setParity(QSerialPort::NoParity);
    mp_serialPort->setStopBits(QSerialPort::OneStop);
    mp_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (!mp_serialPort->isOpen()) {
        qDebug() << mp_serialPort->portName() << " is not open.";

        if (!mp_serialPort->open(QIODevice::ReadWrite)) {
            qDebug() << "Failed to open serial port:" << mp_serialPort->errorString();
            return;
        }
        else {
            qDebug() << "open serial port sucessfully!!!";
        }
    }
    else {
        qDebug() << mp_serialPort->portName() << " is already open!!!";
    }

    connect(mp_serialPort, &QSerialPort::bytesWritten, this, &SerialCommu::do_handleBytesWritten);
    connect(mp_serialPort, &QSerialPort::readyRead, this, &SerialCommu::do_handleReadyRead);
    connect(&m_timerByteInterval, &QTimer::timeout, this, &SerialCommu::do_handleTimerByteIntervalTimeOut);
    connect(&m_timerFrameInterval, &QTimer::timeout, this, &SerialCommu::do_handleTimerFrameIntervalTimeOut);
}
