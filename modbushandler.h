#pragma once

#include <QDebug>
#include <QModbusRtuSerialMaster>
#include <QObject>
#include <QQueue>
#include <QSerialPort>
#include <QTimer>

#include "modbusrequest.h"
#include "os.h"

class ModbusHandler : public QObject {
    Q_OBJECT

public:
    explicit ModbusHandler(OS *p_os, const QString &portName, QObject *parent = nullptr)
        : QObject(parent)
        , requestInProgress(false)
        , mp_os(p_os) {
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
            qWarning() << "Failed to connect to Modbus device!!!";
        }
        else {
            qDebug() << " connect to Modbus device sucessfully!!!";
        }
    }

    // void sendRequest(const QModbusDataUnit &dataUnit, int serverAddress, QVector<quint16> *valuesPtr = nullptr) {
    //     requestQueue.enqueue(ModbusRequest(dataUnit, serverAddress, valuesPtr));
    //     if (!requestInProgress) {
    //         emit nextRequest();
    //     }
    // }
    void sendRequest(ModbusRequest &request) {
        ++mp_os->m_stat.modbusHander.cnt_sendRequest;
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
            ++mp_os->m_stat.modbusHander.cnt_onStateChanged_ConnectedState;

            qDebug() << "Modbus device connected";
        }
        else if (state == QModbusDevice::UnconnectedState) {
            ++mp_os->m_stat.modbusHander.cnt_onStateChanged_UnconnectedState;

            qWarning() << "Modbus device disconnected";
        }
    }

    void onErrorOccurred(QModbusDevice::Error error) {
        ++mp_os->m_stat.modbusHander.cnt_onErrorOccurred;

        qWarning() << "Modbus error occurred:" << error;
        requestInProgress = false; // Reset the flag in case of error
        emit nextRequest();        // Try to process the next request
    }

    void onReplyFinished() {
        QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
        if (!reply)
            return;

        if (reply->error() == QModbusDevice::NoError) {
            ++mp_os->m_stat.modbusHander.cnt_processNextRequest_Finished;
            mp_os->m_stat.modbusHander.calc_percent_Finished();

            // const QModbusDataUnit unit = reply->result();
            // qDebug() << "Response received:" << unit.values();

            // Store the result values in the external variable if provided
            if (currentRequest.mp_mbDataUnitReply) {
                *currentRequest.mp_mbDataUnitReply = reply->result();
            }
        }
        else {
            ++mp_os->m_stat.modbusHander.cnt_processNextRequest_Error;

            qWarning() << "Modbus error occurred:" << reply->error();
        }

        reply->deleteLater();
        requestInProgress = false; // Reset the flag to allow next request
        emit nextRequest();        // Signal to process the next request
    }

private:
    void processNextRequest() {
        if (!requestInProgress && !requestQueue.isEmpty()) {
            ++mp_os->m_stat.modbusHander.cnt_processNextRequest;
            mp_os->m_stat.modbusHander.cnt_unFinished = requestQueue.size();

            requestInProgress = true;
            currentRequest = requestQueue.dequeue();

            QModbusReply *reply;
            switch (currentRequest.m_mbRequestType) {
            case MODBUS_REQUEST_READ:
                ++mp_os->m_stat.modbusHander.cnt_processNextRequest_Read;
                reply = modbusClient->sendReadRequest(currentRequest.m_mbDataUnitRequest,
                                                      currentRequest.m_serverAddress);
                break;
            case MODBUS_REQUEST_WRITE:
                ++mp_os->m_stat.modbusHander.cnt_processNextRequest_Write;
                reply = modbusClient->sendWriteRequest(currentRequest.m_mbDataUnitRequest,
                                                       currentRequest.m_serverAddress);
                break;
            case MODBUS_REQUEST_RW:
                ++mp_os->m_stat.modbusHander.cnt_processNextRequest_RW;
                reply = nullptr;
                break;
            default:
                ++mp_os->m_stat.modbusHander.cnt_processNextRequest_NONE;
                reply = nullptr;
                break;
            }

            if (reply) {
                connect(reply, &QModbusReply::finished, this, &ModbusHandler::onReplyFinished);
            }
            else {
                ++mp_os->m_stat.modbusHander.cnt_processNextRequest_NULL;

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

    //依赖注入
    OS *mp_os;
};
