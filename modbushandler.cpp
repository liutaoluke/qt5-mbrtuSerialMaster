#include "modbushandler.h"

ModbusHandler::ModbusHandler(OS *p_os, const QString &portName, QObject *parent)
    : QObject(parent)
    , mp_os(p_os)
    , requestInProgress(false) {
    modbusClient = new QModbusRtuSerialMaster(this);

    connect(modbusClient, &QModbusClient::stateChanged, this, &ModbusHandler::onStateChanged);
    connect(modbusClient, &QModbusClient::errorOccurred, this, &ModbusHandler::onErrorOccurred);
    connect(this, &ModbusHandler::nextRequest, this, &ModbusHandler::processNextRequest);

    modbusClient->setConnectionParameter(QModbusDevice::SerialPortNameParameter, portName);
    modbusClient->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
                                         QSerialPort::Baud9600);
    modbusClient->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, QSerialPort::Data8);
    modbusClient->setConnectionParameter(QModbusDevice::SerialParityParameter,
                                         QSerialPort::NoParity);
    modbusClient->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
                                         QSerialPort::OneStop);

    modbusClient->setTimeout(1000);
    modbusClient->setNumberOfRetries(3);
}

ModbusHandler::~ModbusHandler() {
    qCritical() << "destructor - ModbusHandler::~ModbusHandler()!!!";
    qDebug() << "destructor - ModbusHandler::~ModbusHandler - ThreadId: "
             << QThread::currentThreadId();
}
