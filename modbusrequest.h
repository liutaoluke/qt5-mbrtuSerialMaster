#pragma once

#include <QModbusDataUnit>
#include <QVector>

struct ModbusRequest {
    QModbusDataUnit dataUnit;
    int serverAddress;
    QVector<quint16> *mp_mbDataUnitValues; // Pointer to store the result values

    // Default constructor
    // ModbusRequest()
    //     : dataUnit()
    //     , serverAddress(-1)
    //     , mp_mbDataUnitValues(nullptr) {}
    ModbusRequest() = default;

    // Parameterized constructor
    ModbusRequest(const QModbusDataUnit &dataUnit, int serverAddress, QVector<quint16> *valuesPtr = nullptr)
        : dataUnit(dataUnit)
        , serverAddress(serverAddress)
        , mp_mbDataUnitValues(valuesPtr) {}

    // Copy constructor
    ModbusRequest(const ModbusRequest &other)
        : dataUnit(other.dataUnit)
        , serverAddress(other.serverAddress)
        , mp_mbDataUnitValues(other.mp_mbDataUnitValues) {}

    // Assignment operator
    ModbusRequest &operator=(const ModbusRequest &other) {
        if (this != &other) {
            dataUnit = other.dataUnit;
            serverAddress = other.serverAddress;
            mp_mbDataUnitValues = other.mp_mbDataUnitValues;
        }
        return *this;
    }

    // Move constructor
    ModbusRequest(ModbusRequest &&other) noexcept
        : dataUnit(std::move(other.dataUnit))
        , serverAddress(other.serverAddress)
        , mp_mbDataUnitValues(other.mp_mbDataUnitValues) {
        other.mp_mbDataUnitValues = nullptr;
    }

    // Move assignment operator
    ModbusRequest &operator=(ModbusRequest &&other) noexcept {
        if (this != &other) {
            dataUnit = std::move(other.dataUnit);
            serverAddress = other.serverAddress;
            mp_mbDataUnitValues = other.mp_mbDataUnitValues;
            other.mp_mbDataUnitValues = nullptr;
        }
        return *this;
    }
};
