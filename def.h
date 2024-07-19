#pragma once

// #define QT_NO_DEBUG_OUTPUT

#ifdef QT_NO_DEBUG_OUTPUT
#define qDebug(...) ((void) 0)
#endif

/*** modbus_handler ***/
enum modbusHandler_enDT { MODBUS_CLIENT_TIMEOUT = 1000 };

/*** serial_communication ***/
enum serialCommu_enDT {
    SERIAL_COMMU_BYTE_INTVL_TIMEOUT = 5,
    SERIAL_COMMU_FRAME_INTVL_TIMEOUT = 1500,
    SERIAL_COMMU_NUMBER_OF_RETRIES = 3
};
