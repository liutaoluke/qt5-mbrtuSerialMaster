#pragma once
#include "def.h"

#include <QDebug>
#include <QModbusDataUnit>
#include <QThread>
#include <QVector>

enum ModbusRequestType_enDT { MODBUS_REQUEST_READ, MODBUS_REQUEST_WRITE, MODBUS_REQUEST_RW };

struct ModbusRequest {
    ModbusRequestType_enDT m_mbRequestType;
    int m_serverAddress;
    QModbusDataUnit m_mbDataUnitRequest;
    QModbusDataUnit *mp_mbDataUnitReply; // Pointer to store the result

    // Default constructor
    // ModbusRequest()
    //     : m_serverAddress(-1)
    //     , m_mbDataUnitRequest()
    //     , mp_mbDataUnitReply(nullptr) {}
    explicit ModbusRequest() = default;

    // Parameterized constructor
    explicit ModbusRequest(ModbusRequestType_enDT mbRequestType,
                           int serverAddress,
                           const QModbusDataUnit &dataUnitRequest,
                           QModbusDataUnit *p_dataUnitReply = nullptr)
        : m_mbRequestType(mbRequestType)
        , m_serverAddress(serverAddress)
        , m_mbDataUnitRequest(dataUnitRequest)
        , mp_mbDataUnitReply(p_dataUnitReply) {}

    virtual ~ModbusRequest() {
        // qCritical() << "destructor - ModbusRequest::~ModbusRequest()!!!";
        // qDebug() << "destructor - ModbusRequest::~ModbusRequest - ThreadId: "
        //          << QThread::currentThreadId();
    }

#if 0
    // Copy constructor
    ModbusRequest(const ModbusRequest &other)
        : m_serverAddress(other.m_serverAddress)
        , m_mbDataUnitRequest(other.m_mbDataUnitRequest)
        , mp_mbDataUnitReply(other.mp_mbDataUnitReply) {}

    // Assignment operator
    ModbusRequest &operator=(const ModbusRequest &other) {
        if (this != &other) {
            m_serverAddress = other.m_serverAddress;
            m_mbDataUnitRequest = other.m_mbDataUnitRequest;
            mp_mbDataUnitReply = other.mp_mbDataUnitReply;
        }
        return *this;
    }

    // Move constructor
    ModbusRequest(ModbusRequest &&other) noexcept
        : m_serverAddress(other.m_serverAddress)
        , m_mbDataUnitRequest(std::move(other.m_mbDataUnitRequest))
        , mp_mbDataUnitReply(other.mp_mbDataUnitReply) {
        other.mp_mbDataUnitReply = nullptr;
    }

    // Move assignment operator
    ModbusRequest &operator=(ModbusRequest &&other) noexcept {
        if (this != &other) {
            m_serverAddress = other.m_serverAddress;
            m_mbDataUnitRequest = std::move(other.m_mbDataUnitRequest);
            mp_mbDataUnitReply = other.mp_mbDataUnitReply;
            other.mp_mbDataUnitReply = nullptr;
        }
        return *this;
    }
#endif
};
