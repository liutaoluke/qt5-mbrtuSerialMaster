#pragma once
#include "def.h"

#include <QObject>

#include "meter.h"
#include "modbushandler.h"
#include "os.h"

class ESS : public QObject {
    Q_OBJECT
public:
    explicit ESS(OS *p_os, QObject *parent = nullptr);

    void controlOSQTimerInterval(OS *p_os);

public slots:
    void do_osQTimerInterval_Start(OS *p_os);
    void do_osQTimerInterval_Stop(OS *p_os);
    void do_mbHandlerCom2_01sec();
    void do_mbHandlerCom2_05sec();
    void do_mbHandlerCom2_30sec();
    void do_mbHandlerCom2_02min();
    void do_mbHandlerCom2_10min();

signals:

private:
    ModbusHandler *mp_mbHandlerCom1 = nullptr;
    ModbusHandler *mp_mbHandlerCom2 = nullptr;
    ModbusHandler *mp_mbHandlerCom3 = nullptr;

    OS *mp_osCom1 = nullptr;
    OS *mp_osCom2 = nullptr;
    OS *mp_osCom3 = nullptr;

    Meter *mp_meterGrid = nullptr;
    Meter *mp_meterLoad = nullptr;
    Meter *mp_meterESS = nullptr;
};
