#include "meter.h"

Meter::Meter(int device_address, QObject *parent)
    : QObject{parent}
    , m_deviceAddress(device_address)
    , m_mbDataUnitRequestGetDataVAW(QModbusDataUnit::HoldingRegisters, 0x2000, 82)
    , m_mbDataUnitRequestGetDataEnergy(QModbusDataUnit::HoldingRegisters, 0x101E, 60)
    , m_mbDataUnitRequestGetParaSys(QModbusDataUnit::HoldingRegisters, 0x0000, 53)
    , m_mbDataUnitRequestGetParaTOUA(QModbusDataUnit::HoldingRegisters, 0x6000, 56)
    , m_mbRequestGetDataVAW(m_deviceAddress, m_mbDataUnitRequestGetDataVAW, &m_mbDataUnitReplyGetDataVAW)
    , m_mbRequestGetDataEnergy(m_deviceAddress,
                               m_mbDataUnitRequestGetDataEnergy,
                               &m_mbDataUnitReplyGetDataEnergy)
    , m_mbRequestGetParaSys(m_deviceAddress, m_mbDataUnitRequestGetParaSys, &m_mbDataUnitReplyGetParaSys)
    , m_mbRequestGetParaTOUA(m_deviceAddress, m_mbDataUnitRequestGetParaTOUA, &m_mbDataUnitReplyGetParaTouA) {
}
