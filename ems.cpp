#include "ems.h"

EMS::EMS(QObject *parent)
    : QObject{parent} {
    mp_mbHandlerCom1 = new ModbusHandler(&os[1], "ttyS1", this);
    mp_mbHandlerCom1->start();

    mp_mbHandlerCom2 = new ModbusHandler(&os[2], "ttyS2", this);
    mp_mbHandlerCom2->start();

    mp_mbHandlerCom3 = new ModbusHandler(&os[0], "ttyS8", this);
    mp_mbHandlerCom3->start();

    mp_serialCommuCom3 = new SerialCommu(&os[3], "ttyS4", this);
    mp_serialCommuCom3->start();
}

EMS::~EMS() {
    qCritical() << "destructor - EMS::~EMS()!!!";
    qDebug() << "destructor - EMS::~EMS - ThreadId: " << QThread::currentThreadId();
}
