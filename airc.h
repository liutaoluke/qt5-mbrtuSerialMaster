#pragma once

#include <QObject>
#include "serialrequest.h"

class AirC : public QObject {
    Q_OBJECT
public:
    explicit AirC(QObject *parent = nullptr);
    virtual ~AirC();
signals:

private:
    QByteArray m_byteArrayRequestData01;
    QByteArray m_byteArrayRequestData02;
    QByteArray m_byteArrayRequestData03;

public:
    QByteArray m_byteArrayReplyData01;
    QByteArray m_byteArrayReplyData02;
    QByteArray m_byteArrayReplyData03;

    SerialRequest m_serialRequestData01;
    SerialRequest m_serialRequestData02;
    SerialRequest m_serialRequestData03;
};
