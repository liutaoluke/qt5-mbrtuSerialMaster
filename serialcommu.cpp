#include "serialcommu.h"

#include <QDebug>

SerialCommu::SerialCommu(OS *p_os, const QString &portName, QObject *parent)
    : QObject{parent}
    , m_requestInProgress(false) {
    qDebug() << "constructor - SerialCommu::SerialCommu - ThreadId: " << QThread::currentThreadId();

    mp_serialPort = new QSerialPort(this);

    mp_serialPort->setPortName(portName);
    mp_serialPort->setBaudRate(QSerialPort::Baud9600);
    mp_serialPort->setDataBits(QSerialPort::Data8);
    mp_serialPort->setParity(QSerialPort::NoParity);
    mp_serialPort->setStopBits(QSerialPort::OneStop);
    mp_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    connect(mp_serialPort, &QSerialPort::bytesWritten, this, &SerialCommu::do_handleBytesWritten);
    connect(mp_serialPort, &QSerialPort::readyRead, this, &SerialCommu::do_handleReadyRead);
    connect(&m_timerByteInterval, &QTimer::timeout, this, &SerialCommu::do_handleTimerByteIntervalTimeOut);
    connect(&m_timerFrameInterval, &QTimer::timeout, this, &SerialCommu::do_handleTimerFrameIntervalTimeOut);
}

SerialCommu::~SerialCommu() {
    qCritical() << "destructor - SerialCommu::~SerialCommu()!!!";
    qDebug() << "destructor - SerialCommu::~SerialCommu - ThreadId: " << QThread::currentThreadId();
}
