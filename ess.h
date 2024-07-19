#pragma once
#include "os.h"

#include <QList>
#include <QObject>
#include "airc.h"
#include "ems.h"
#include "meter.h"
#include "ostimer.h"

#define CONTAINER_QLIST

class ESS : public QObject {
    Q_OBJECT
public:
    explicit ESS(QObject *parent = nullptr);
    virtual ~ESS();

    void ssControlOSQTimerInterval_MBHandler(OS *p_os, OSTimer *p_ostimer);
    void ssControlOSQTimerInterval_SerialCommu(OS *p_os, OSTimer *p_ostimer);

public slots:

    void do_initESSInSubThread();

private slots:

    void do_startOSQTimerIntvl(OSTimer *p_ostimer);
    void do_stopOSQTimerIntvl(OSTimer *p_ostimer);
    void do_emsMBHandlerCom2_01sec();
    void do_emsMBHandlerCom2_05sec();
    void do_emsMBHandlerCom2_30sec();
    void do_emsMBHandlerCom2_02min();
    void do_emsMBHandlerCom2_10min();

    void do_emsSerialCommuCom3_01sec();
signals:

public:
#ifndef CONTAINER_QLIST
    OSTimer *mp_ostimer = nullptr;
#else
    QList<OSTimer *> mlist_p_ostimer;
#endif

    EMS *mp_ems = nullptr;

    Meter *mp_meterGrid = nullptr;
    Meter *mp_meterLoad = nullptr;
    Meter *mp_meterESS = nullptr;

    AirC *mp_ariC = nullptr;
};
