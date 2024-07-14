#include "ess.h"

ESS::ESS(OS *p_os, QObject *parent)
    : QObject{parent} {
    mp_osCom1 = p_os + 0;
    mp_osCom2 = p_os + 1;
    mp_osCom3 = p_os + 2;

    mp_mbHandlerCom1 = new ModbusHandler(mp_osCom1, "/dev/ttyS1");
    mp_mbHandlerCom1->start();

    mp_mbHandlerCom2 = new ModbusHandler(mp_osCom2, "/dev/ttyS2");
    mp_mbHandlerCom2->start();

    mp_mbHandlerCom3 = new ModbusHandler(mp_osCom3, "/dev/ttyS4");
    mp_mbHandlerCom3->start();

    mp_meterGrid = new Meter(21, this);
    mp_meterLoad = new Meter(22, this);
    mp_meterESS = new Meter(23, this);

    controlOSQTimerInterval(mp_osCom1);
    controlOSQTimerInterval(mp_osCom2);
    controlOSQTimerInterval(mp_osCom3);
    connect(&mp_osCom2->m_interval.timer_01sec, &QTimer::timeout, this, &ESS::do_mbHandlerCom2_01sec);
    connect(&mp_osCom2->m_interval.timer_05sec, &QTimer::timeout, this, &ESS::do_mbHandlerCom2_05sec);
    connect(&mp_osCom2->m_interval.timer_02min, &QTimer::timeout, this, &ESS::do_mbHandlerCom2_02min);
    connect(&mp_osCom2->m_interval.timer_10min, &QTimer::timeout, this, &ESS::do_mbHandlerCom2_10min);
}

void ESS::controlOSQTimerInterval(OS *p_os) {
    QTimer *p_timer = new QTimer(this);
    p_timer->start(OS_QTIMER_INTERVAL_UPDATE);
    connect(p_timer, &QTimer::timeout, this, [=]() {
        if (p_os->m_stat.modbusHander.cnt_unFinished > OS_STAT_MBH_CNT_UNFINISHED_MAX) {
            do_osQTimerInterval_Stop(p_os);
        }
        else if (p_os->m_stat.modbusHander.cnt_unFinished < OS_STAT_MBH_CNT_UNFINISHED_MIN) {
            do_osQTimerInterval_Start(p_os);
        }
    });
}

void ESS::do_osQTimerInterval_Start(OS *p_os) {
    if (!p_os->m_interval.timer_01sec.isActive())
        p_os->m_interval.timer_01sec.start(OS_QTIMER_INTERVAL_01SEC);
    if (!p_os->m_interval.timer_05sec.isActive())
        p_os->m_interval.timer_05sec.start(OS_QTIMER_INTERVAL_05SEC);
    // if (!p_os->m_interval.timer_30sec.isActive())
    //     p_os->m_interval.timer_30sec.start(OS_QTIMER_INTERVAL_30SEC);
    // if (!p_os->m_interval.timer_02min.isActive())
    //     p_os->m_interval.timer_02min.start(OS_QTIMER_INTERVAL_02MIN);
    // if (!p_os->m_interval.timer_10min.isActive())
    //     p_os->m_interval.timer_10min.start(OS_QTIMER_INTERVAL_10MIN);
}

void ESS::do_osQTimerInterval_Stop(OS *p_os) {
    p_os->m_interval.timer_01sec.stop();
    p_os->m_interval.timer_05sec.stop();
    // p_os->m_interval.timer_30sec.stop();
    // p_os->m_interval.timer_02min.stop();
    // p_os->m_interval.timer_10min.stop();
}

void ESS::do_mbHandlerCom2_01sec() {
    ++mp_osCom2->m_stat.qtimer.cnt_01sec;
    mp_mbHandlerCom2->sendRequest(mp_meterGrid->m_mbRequestGetDataVAW);
    mp_mbHandlerCom2->sendRequest(mp_meterLoad->m_mbRequestGetDataVAW);
    mp_mbHandlerCom2->sendRequest(mp_meterESS->m_mbRequestGetDataVAW);
    qDebug() << " meterGrid.m_mbRequestGetDataVAW : " << mp_meterGrid->m_mbDataUnitReplyGetDataVAW.values();
    qDebug() << " meterLoad.m_mbRequestGetDataVAW : " << mp_meterLoad->m_mbDataUnitReplyGetDataVAW.values();
    qDebug() << "  meterEss.m_mbRequestGetDataVAW : " << mp_meterESS->m_mbDataUnitReplyGetDataVAW.values();
}

void ESS::do_mbHandlerCom2_05sec() {
    ++mp_osCom2->m_stat.qtimer.cnt_05sec;
    mp_mbHandlerCom2->sendRequest(mp_meterGrid->m_mbRequestGetDataEnergy);
    mp_mbHandlerCom2->sendRequest(mp_meterLoad->m_mbRequestGetDataEnergy);
    mp_mbHandlerCom2->sendRequest(mp_meterESS->m_mbRequestGetDataEnergy);
    qDebug() << " meterGrid.m_mbRequestGetDataEnergy : "
             << mp_meterGrid->m_mbDataUnitReplyGetDataEnergy.values();
    qDebug() << " meterLoad.m_mbRequestGetDataEnergy : "
             << mp_meterLoad->m_mbDataUnitReplyGetDataEnergy.values();
    qDebug() << "  meterEss.m_mbRequestGetDataEnergy : "
             << mp_meterESS->m_mbDataUnitReplyGetDataEnergy.values();
}

void ESS::do_mbHandlerCom2_30sec() {}

void ESS::do_mbHandlerCom2_02min() {
    ++mp_osCom2->m_stat.qtimer.cnt_02min;
    mp_mbHandlerCom2->sendRequest(mp_meterGrid->m_mbRequestGetParaSys);
    mp_mbHandlerCom2->sendRequest(mp_meterLoad->m_mbRequestGetParaSys);
    mp_mbHandlerCom2->sendRequest(mp_meterESS->m_mbRequestGetParaSys);
    qDebug() << " meterGrid.m_mbRequestGetParaSys : " << mp_meterGrid->m_mbDataUnitReplyGetParaSys.values();
    qDebug() << " meterLoad.m_mbRequestGetParaSys : " << mp_meterLoad->m_mbDataUnitReplyGetParaSys.values();
    qDebug() << "  meterEss.m_mbRequestGetParaSys : " << mp_meterESS->m_mbDataUnitReplyGetParaSys.values();
}

void ESS::do_mbHandlerCom2_10min() {
    ++mp_osCom2->m_stat.qtimer.cnt_10min;
    mp_mbHandlerCom2->sendRequest(mp_meterGrid->m_mbRequestGetParaTOUA);
    mp_mbHandlerCom2->sendRequest(mp_meterLoad->m_mbRequestGetParaTOUA);
    mp_mbHandlerCom2->sendRequest(mp_meterESS->m_mbRequestGetParaTOUA);
    qDebug() << " meterGrid.m_mbRequestGetParaTOUA : " << mp_meterGrid->m_mbDataUnitReplyGetParaTouA.values();
    qDebug() << " meterLoad.m_mbRequestGetParaTOUA : " << mp_meterLoad->m_mbDataUnitReplyGetParaTouA.values();
    qDebug() << "  meterEss.m_mbRequestGetParaTOUA : " << mp_meterESS->m_mbDataUnitReplyGetParaTouA.values();
}
