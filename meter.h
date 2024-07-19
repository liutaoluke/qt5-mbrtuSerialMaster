#pragma once
#include "os.h"

#include <QDebug>
#include <QModbusDataUnit>
#include <QObject>
#include "modbusrequest.h"

class Meter : public QObject {
    Q_OBJECT
public:
    explicit Meter(int device_address = -1, QObject *parent = nullptr);
    virtual ~Meter();

signals:

private:
    int m_deviceAddress;

    const QModbusDataUnit m_mbDataUnitRequestGetDataVAW;
    const QModbusDataUnit m_mbDataUnitRequestGetDataEnergy;
    const QModbusDataUnit m_mbDataUnitRequestGetParaSys;
    const QModbusDataUnit m_mbDataUnitRequestGetParaTOUA;

public:
    QModbusDataUnit m_mbDataUnitReplyGetDataVAW;
    QModbusDataUnit m_mbDataUnitReplyGetDataEnergy;
    QModbusDataUnit m_mbDataUnitReplyGetParaSys;
    QModbusDataUnit m_mbDataUnitReplyGetParaTouA;

    ModbusRequest m_mbRequestGetDataVAW;
    ModbusRequest m_mbRequestGetDataEnergy;
    ModbusRequest m_mbRequestGetParaSys;
    ModbusRequest m_mbRequestGetParaTOUA;
};
