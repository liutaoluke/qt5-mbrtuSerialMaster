#pragma once

#include <QDebug>
#include <QModbusRtuSerialMaster>
#include <QObject>
#include <QQueue>
#include <QSerialPort>
#include <QTimer>

#include "modbusrequest.h"

class ModbusHandler : public QObject {
    Q_OBJECT

public:
    explicit ModbusHandler(const QString &portName, QObject *parent = nullptr)
        : QObject(parent)
        , requestInProgress(false) {
        modbusClient = new QModbusRtuSerialMaster(this);

        connect(modbusClient, &QModbusClient::stateChanged, this, &ModbusHandler::onStateChanged);
        connect(modbusClient, &QModbusClient::errorOccurred, this, &ModbusHandler::onErrorOccurred);
        connect(this, &ModbusHandler::nextRequest, this, &ModbusHandler::processNextRequest);

        modbusClient->setConnectionParameter(QModbusDevice::SerialPortNameParameter, portName);
        modbusClient->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, QSerialPort::Baud9600);
        modbusClient->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, QSerialPort::Data8);
        modbusClient->setConnectionParameter(QModbusDevice::SerialParityParameter, QSerialPort::NoParity);
        modbusClient->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, QSerialPort::OneStop);

        modbusClient->setTimeout(1000);
        modbusClient->setNumberOfRetries(3);
    }

    void start() {
        if (!modbusClient->connectDevice()) {
            qWarning() << "Failed to connect to Modbus device";
        }
    }

    // void sendRequest(const QModbusDataUnit &dataUnit, int serverAddress, QVector<quint16> *valuesPtr = nullptr) {
    //     requestQueue.enqueue(ModbusRequest(dataUnit, serverAddress, valuesPtr));
    //     if (!requestInProgress) {
    //         emit nextRequest();
    //     }
    // }
    void sendRequest(ModbusRequest &request) {
        requestQueue.enqueue(request);
        if (!requestInProgress) {
            emit nextRequest();
        }
    }

signals:
    void nextRequest();

private slots:
    void onStateChanged(QModbusDevice::State state) {
        if (state == QModbusDevice::ConnectedState) {
            qDebug() << "Modbus device connected";
        }
        else if (state == QModbusDevice::UnconnectedState) {
            qWarning() << "Modbus device disconnected";
        }
    }

    void onErrorOccurred(QModbusDevice::Error error) {
        qWarning() << "Modbus error occurred:" << error;
        requestInProgress = false; // Reset the flag in case of error
        emit nextRequest();        // Try to process the next request
    }

    void onReplyFinished() {
        QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
        if (!reply)
            return;

        if (reply->error() == QModbusDevice::NoError) {
            // const QModbusDataUnit unit = reply->result();
            // qDebug() << "Response received:" << unit.values();

            // Store the result values in the external variable if provided
            if (currentRequest.mp_mbDataUnitReply) {
                *currentRequest.mp_mbDataUnitReply = reply->result();
            }
        }
        else {
            qWarning() << "Modbus error occurred:" << reply->error();
        }

        reply->deleteLater();
        requestInProgress = false; // Reset the flag to allow next request
        emit nextRequest();        // Signal to process the next request
    }

private:
    void processNextRequest() {
        if (!requestInProgress && !requestQueue.isEmpty()) {
            requestInProgress = true;
            currentRequest = requestQueue.dequeue();
            QModbusReply *reply = modbusClient->sendReadRequest(currentRequest.dataUnit, currentRequest.serverAddress);
            if (reply) {
                connect(reply, &QModbusReply::finished, this, &ModbusHandler::onReplyFinished);
            }
            else {
                qWarning() << "Failed to send request";
                requestInProgress = false;
                emit nextRequest();
            }
        }
    }

    QModbusRtuSerialMaster *modbusClient;
    QQueue<ModbusRequest> requestQueue;
    ModbusRequest currentRequest;
    bool requestInProgress;
};
