#pragma once
#include "def.h"

#include <QByteArray>

struct SerialRequest {
public:
    explicit SerialRequest() = default;
    explicit SerialRequest(QByteArray dataRequest)
        : m_dataRequest(dataRequest){};

public:
    QByteArray m_dataRequest;
    QByteArray m_dataReply;
};
