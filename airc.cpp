#include "airc.h"
#include <QThread>

AirC::AirC(QObject *parent)
    : QObject{parent} {
    m_byteArrayRequestData01 = "AirC Request Data01";
    m_byteArrayRequestData02 = "AirC Request Data02";
    m_byteArrayRequestData03 = "AirC Request Data03";

    m_serialRequestData01 = SerialRequest(m_byteArrayRequestData01, &m_byteArrayReplyData01);
    m_serialRequestData02 = SerialRequest(m_byteArrayRequestData02, &m_byteArrayReplyData02);
    m_serialRequestData03 = SerialRequest(m_byteArrayRequestData03, &m_byteArrayReplyData03);
}

AirC::~AirC() {
    qCritical() << "destructor - AirC::~AirC()!!!";
    qDebug() << "destructor - AirC::~AirC - ThreadId: " << QThread::currentThreadId();
}
