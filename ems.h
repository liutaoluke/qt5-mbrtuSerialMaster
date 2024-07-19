#pragma once
#include "os.h"

#include <QObject>

#include "modbushandler.h"
#include "serialcommu.h"

class EMS : public QObject {
    Q_OBJECT
public:
    explicit EMS(QObject *parent = nullptr);
    virtual ~EMS();

signals:

public:
    ModbusHandler *mp_mbHandlerCom1 = nullptr;
    ModbusHandler *mp_mbHandlerCom2 = nullptr;
    ModbusHandler *mp_mbHandlerCom3 = nullptr;

    SerialCommu *mp_serialCommuCom3 = nullptr;
};
