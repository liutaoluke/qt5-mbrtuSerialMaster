#pragma once
#include "def.h"

#include <QByteArray>
#include <QDebug>
#include <QThread>

struct SerialRequest {
public:
    explicit SerialRequest() = default;

    explicit SerialRequest(QByteArray dataRequest, QByteArray *p_dataReply)
        : m_dataRequest(dataRequest)
        , mp_dataReply(p_dataReply){};

    virtual ~SerialRequest() {
        // qCritical() << "destructor - SerialRequest::~SerialRequest()!!!";
        // qDebug() << "destructor - SerialRequest::~SerialRequest - ThreadId: "
        //          << QThread::currentThreadId();
    }

public:
    QByteArray m_dataRequest;
    QByteArray *mp_dataReply = nullptr;
};
