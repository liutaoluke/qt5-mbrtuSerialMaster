#pragma once
#include "def.h"

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
    explicit SerialCommu(QObject *parent = nullptr);

public:
    void sendRequest(const SerialRequest &request) {
        m_requestQueue.enqueue(request);
        if (!m_requestInProgress) {
            processNextRequest(); // 处理队列中的请求
        }
    }

signals:

private slots:
    void do_handleBytesWritten(quint64 bytes) {
        qDebug() << "SerialCommu::do_handleBytesWritten - ThreadId: " << QThread::currentThreadId();
        if (bytes == static_cast<quint64>(m_currentRequest.m_dataRequest.size())) {
            qDebug() << "Data sent successfully!";

            // 可以进行接收数据的准备或其他操作

            //只要本次请求发送正确，则开启数据帧接收超时判断
            m_timerFrameInterval.start(m_frameIntervalTimeOut);
        }
        else {
            qDebug() << "Data sent failed!";

            //本次请求发送失败，直接执行下一次请求；
            m_requestInProgress = false;
            processNextRequest();
        }
    }
    void do_handleReadyRead() {
        //只要接收到一个字节，则关闭数据帧接收超时判断
        m_timerFrameInterval.stop();
        //只要接收到一个字节，则复位设置超时相关变量：超时标志、重试计数
        m_cntOfRetries = 0;
        m_flagFrameIntervalTimeOut = false;

        //同时开启字节间隔定时器
        m_timerByteInterval.start(m_byteIntervalTimeOut);

        QByteArray replyData = mp_serialPort->readAll();
        qDebug() << "Received data *byte* from serial port:" << replyData;
        // 在这里处理接收到的数据，根据需求进行解析和处理

        m_currentRequest.m_dataReply.append(replyData);
    }
    void do_handleTimerByteIntervalTimeOut() {
        qDebug() << "SerialCommu::do_handleTimerByteIntervalTimeOut - ThreadId: "
                 << QThread::currentThreadId();
        //字节间隔定时超时，则判断为数据帧接收完毕，可以进行数据帧的相关的处理
        m_timerByteInterval.stop();

        if (!m_currentRequest.m_dataReply.isEmpty()) {
            qDebug() << "Received data *Frame* from serial port:" << m_currentRequest.m_dataReply;
            m_currentRequest.m_dataReply.clear();
        }
        else {
            qDebug() << "not Received data from serial Port, error：m_dataReply is null";
        }

        m_requestInProgress = false;
        processNextRequest();
    }
    void do_handleTimerFrameIntervalTimeOut() {
        qDebug() << "not Received data from serial Port, error：timerFrame is timeout";
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
            qDebug() << "Sending serial communication data:" << requestData;
        }
    }

private:
    QSerialPort *mp_serialPort;
    SerialRequest m_currentRequest;
    bool m_requestInProgress;

    //首先进行数据帧超时判断，如果数据帧超时，则丢弃此次请求直接进行下一个请求，当然也可以设置重发机制，多次尝试；
    //如果数据帧没有超时，则启动字节间隔超时判断，用来确定帧与帧之间间隔，提取出来数据帧进行相关的数据处理；
    //帧间超时，用于判断是否启动重发机制；
    //字节间超时，用于提取数据帧，然后进行帧校验，校验通过进行数据提取与逻辑处理；
    QTimer m_timerByteInterval;
    const int m_byteIntervalTimeOut = 5;
    QTimer m_timerFrameInterval;
    const int m_frameIntervalTimeOut = 1500;
    int m_cntOfRetries = 0;
    const int m_numberOfRetries = 3;
    bool m_flagFrameIntervalTimeOut = false;

public:
    QQueue<SerialRequest> m_requestQueue;
};
