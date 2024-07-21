#pragma once

// #define QT_NO_DEBUG_OUTPUT

#ifdef QT_NO_DEBUG_OUTPUT
#define qDebug(...) ((void) 0)
#endif

/*** os timer ***/
//QTimerINTVL
enum OS_Timer_Intvl_enDT {

    OS_TIMER_INTVL_01SEC = 500,
    OS_TIMER_INTVL_05SEC = 1000,
    OS_TIMER_INTVL_30SEC = 30000,
    OS_TIMER_INTVL_02MIN = 120000,
    OS_TIMER_INTVL_10MIN = 600000,

    OS_TIMER_INTVL_UPDATE = OS_TIMER_INTVL_01SEC * 2,
};

/*** modbus_handler ***/
enum modbusHandler_enDT { MODBUS_CLIENT_TIMEOUT = 1000 };

/*** serial_communication ***/
enum serialCommu_enDT {
    SERIAL_COMMU_BYTE_INTVL_TIMEOUT = 5,
    SERIAL_COMMU_FRAME_INTVL_TIMEOUT = 1500,
    SERIAL_COMMU_NUMBER_OF_RETRIES = 3
};

/*** ui timer intvl ***/
enum UI_Timer_Intvl_enDT {
    UI_TIMER_INTVL_UPDATE = OS_TIMER_INTVL_UPDATE,
};
