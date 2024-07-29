#pragma once
#include "os.h"

#include <QDebug>
#include <QObject>
#include <QQueue>
#include <QSerialPort>
#include <QThread>
#include <QTimer>

#include "serialrequest.h"

class SerialCommu : public QObject {
    Q_OBJECT
public:
    explicit SerialCommu(OS *p_os, const QString &portName, QObject *parent = nullptr);
    virtual ~SerialCommu();

public:
    void start() {
        if (!mp_serialPort->isOpen()) {
            qInfo() << mp_serialPort->portName() << " is not open.";

            if (!mp_serialPort->open(QIODevice::ReadWrite)) {
                qCritical() << "SerialCommu - Failed to open serial port:" << mp_serialPort->errorString();
                return;
            }
            else {
                qInfo() << "SerialCommu - open serial port sucessfully!!!";
            }
        }
        else {
            qWarning() << "SerialCommu - " << mp_serialPort->portName() << " is already open!!!";
        }
    }

    void sendRequest(const SerialRequest &request) {
        // qDebug() << "SerialCommu::sendRequest - ThreadId: " << QThread::currentThreadId();
        m_requestQueue.enqueue(request);
        if (!m_requestInProgress) {
            processNextRequest(); // 处理队列中的请求
        }
    }

signals:

private slots:
    void do_handleBytesWritten(quint64 bytes) {
        // qDebug() << "SerialCommu::do_handleBytesWritten - ThreadId: " << QThread::currentThreadId();
        if (bytes == static_cast<quint64>(m_currentRequest.m_dataRequest.size())) {
            qInfo() << "Data sent successfully!";

            // 可以进行接收数据的准备或其他操作

            //只要本次请求发送正确，则开启数据帧接收超时判断
            m_timerFrameInterval.start(m_frameIntervalTimeOut);
        }
        else {
            qWarning() << "SerialCommu - Data sent failed!";

            //本次请求发送失败，直接执行下一次请求；
            m_requestInProgress = false;
            processNextRequest();
        }
    }
    void do_handleReadyRead() {
        // qDebug() << "SerialCommu::do_handleReadyRead - ThreadId: " << QThread::currentThreadId();

        if (m_currentRequest.mp_dataReply) {
            //只要接收到一个字节，则关闭数据帧接收超时判断
            m_timerFrameInterval.stop();
            //只要接收到一个字节，则复位设置超时相关变量：超时标志、重试计数
            m_cntOfRetries = 0;
            m_flagFrameIntervalTimeOut = false;

            //同时开启字节间隔定时器
            m_timerByteInterval.start(m_byteIntervalTimeOut);

            QByteArray replyData = mp_serialPort->readAll();
            // qDebug() << "SerialCommu - Received data *byte* from serial port:" << replyData;
            // 在这里处理接收到的数据，根据需求进行解析和处理

            // m_currentRequest.mp_dataReply->append(replyData);
            m_currentRequest_dataReplyTemp.append(replyData);
        }
        else {
            qCritical() << "SerialCommu::do_handleReadyRead - m_currentRequest.mp_dataReply is null !!!";
        }
    }
    void do_handleTimerByteIntervalTimeOut() {
        // qDebug() << "SerialCommu::do_handleTimerByteIntervalTimeOut - ThreadId: "
        //          << QThread::currentThreadId();

        if (m_currentRequest.mp_dataReply) {
            //字节间隔定时超时，则判断为数据帧接收完毕，可以进行数据帧的相关的处理
            m_timerByteInterval.stop();

            if (!m_currentRequest_dataReplyTemp.isEmpty()) {
                *m_currentRequest.mp_dataReply = m_currentRequest_dataReplyTemp;
                // qDebug() << "SerialCommu - Received data *Frame* from serial port:"
                //          << *m_currentRequest.mp_dataReply;
                m_currentRequest_dataReplyTemp.clear();
            }
            else {
                qWarning()
                    << "SerialCommu - not Received data from serial Port, error：m_dataReply is empty!!!";
            }

            m_requestInProgress = false;
            processNextRequest();
        }
        else {
            qCritical() << "SerialCommu::do_handleTimerByteIntervalTimeOut - m_currentRequest.mp_dataReply "
                           "is null !!!";
        }
    }
    void do_handleTimerFrameIntervalTimeOut() {
        qWarning() << "SerialCommu - not Received data from serial Port, error：timerFrame is timeout";
        m_timerFrameInterval.stop();

        m_flagFrameIntervalTimeOut = true;
        ++m_cntOfRetries;

        //本次请求接收超时，直接执行下一次请求；
        m_requestInProgress = false;
        processNextRequest();
    }

private:
    void processNextRequest() {
        if ((!m_requestInProgress) && (!m_requestQueue.isEmpty())) {
            m_requestInProgress = true;

            //如果数据帧不超时，或者发送次数大于设定重试次数，则发送下一个队列请求，否则进行重发；
            //不管是正常还是重试超过设定次数，都需要复位设置超时相关变量：超时标志、重试计数
            if ((!m_flagFrameIntervalTimeOut) || (m_cntOfRetries >= m_numberOfRetries)) {
                m_cntOfRetries = 0;
                m_flagFrameIntervalTimeOut = false;

                m_currentRequest = m_requestQueue.dequeue();
            }

            QByteArray requestData = m_currentRequest.m_dataRequest;
            mp_serialPort->write(requestData);
            // qDebug() << "SerialCommu - Sending serial communication data:" << requestData;
        }
    }

private:
    QSerialPort *mp_serialPort;
    SerialRequest m_currentRequest;
    QByteArray m_currentRequest_dataReplyTemp;
    bool m_requestInProgress;

    //首先进行数据帧超时判断，如果数据帧超时，则丢弃此次请求直接进行下一个请求，当然也可以设置重发机制，多次尝试；
    //如果数据帧没有超时，则启动字节间隔超时判断，用来确定帧与帧之间间隔，提取出来数据帧进行相关的数据处理；
    //帧间超时，用于判断是否启动重发机制；
    //字节间超时，用于提取数据帧，然后进行帧校验，校验通过进行数据提取与逻辑处理；
    QTimer m_timerByteInterval;
    const int m_byteIntervalTimeOut = SERIAL_COMMU_BYTE_INTVL_TIMEOUT;
    QTimer m_timerFrameInterval;
    const int m_frameIntervalTimeOut = SERIAL_COMMU_FRAME_INTVL_TIMEOUT;
    int m_cntOfRetries = 0;
    const int m_numberOfRetries = SERIAL_COMMU_NUMBER_OF_RETRIES;
    bool m_flagFrameIntervalTimeOut = false;

public:
    QQueue<SerialRequest> m_requestQueue;
};
