#include "ess.h"

ESS::ESS(QObject *parent)
    : QObject{parent} {
}

ESS::~ESS() {
    qCritical() << "destructor - ESS::~ESS()!!!";
    qDebug() << "destructor - ESS::~ESS - ThreadId: " << QThread::currentThreadId();

#ifndef CONTAINER_QLIST
    delete[] mp_ostimer;
#endif
}

void ESS::do_initESSInSubThread() {
    qDebug() << "ESS::do_initESSInSubThread - ThreadId: " << QThread::currentThreadId();

    mp_ems = new EMS(this);
    /*com2*/
    mp_meterGrid = new Meter(21, this);
    mp_meterLoad = new Meter(22, this);
    mp_meterESS = new Meter(23, this);
    /*com3*/
    mp_ariC = new AirC(this);

#ifndef CONTAINER_QLIST
    mp_ostimer = new OSTimer[5];
#else
    for (int i = 0; i < 5; ++i) {
        mlist_p_ostimer.append(new OSTimer(this));
    }
#endif

#ifndef CONTAINER_QLIST
    connect(&mlist_p_ostimer.at(2)->m_intvl_01sec,
            &QTimer::timeout,
            this,
            &ESS::do_emsMBHandlerCom2_01sec);
    connect(&mlist_p_ostimer.at(2)->m_intvl_05sec,
            &QTimer::timeout,
            this,
            &ESS::do_emsMBHandlerCom2_05sec);
    connect(&mlist_p_ostimer.at(2)->m_intvl_30sec,
            &QTimer::timeout,
            this,
            &ESS::do_emsMBHandlerCom2_30sec);
    connect(&mlist_p_ostimer.at(2)->m_intvl_02min,
            &QTimer::timeout,
            this,
            &ESS::do_emsMBHandlerCom2_02min);
    connect(&mlist_p_ostimer.at(2)->m_intvl_10min,
            &QTimer::timeout,
            this,
            &ESS::do_emsMBHandlerCom2_10min);

    ssControlOSQTimerInterval_MBHandler(&os[1], mp_ostimer + 1);
    ssControlOSQTimerInterval_MBHandler(&os[2], mp_ostimer + 2);
    ssControlOSQTimerInterval_MBHandler(&os[0], mp_ostimer + 0);

    connect(&(mp_ostimer + 3)->m_intvl_01sec,
            &QTimer::timeout,
            this,
            &ESS::do_emsSerialCommuCom3_01sec);
    ssControlOSQTimerInterval_SerialCommu(&os[3], mp_ostimer + 3);
#else
    connect(&mlist_p_ostimer.at(2)->m_intvl_01sec,
            &QTimer::timeout,
            this,
            &ESS::do_emsMBHandlerCom2_01sec);
    connect(&mlist_p_ostimer.at(2)->m_intvl_05sec,
            &QTimer::timeout,
            this,
            &ESS::do_emsMBHandlerCom2_05sec);
    connect(&mlist_p_ostimer.at(2)->m_intvl_30sec,
            &QTimer::timeout,
            this,
            &ESS::do_emsMBHandlerCom2_30sec);
    connect(&mlist_p_ostimer.at(2)->m_intvl_02min,
            &QTimer::timeout,
            this,
            &ESS::do_emsMBHandlerCom2_02min);
    connect(&mlist_p_ostimer.at(2)->m_intvl_10min,
            &QTimer::timeout,
            this,
            &ESS::do_emsMBHandlerCom2_10min);

    ssControlOSQTimerInterval_MBHandler(&os[1], mlist_p_ostimer.at(1));
    ssControlOSQTimerInterval_MBHandler(&os[2], mlist_p_ostimer.at(2));
    ssControlOSQTimerInterval_MBHandler(&os[0], mlist_p_ostimer.at(0));

    connect(&mlist_p_ostimer.at(3)->m_intvl_01sec,
            &QTimer::timeout,
            this,
            &ESS::do_emsSerialCommuCom3_01sec);
    ssControlOSQTimerInterval_SerialCommu(&os[3], mlist_p_ostimer.at(3));
#endif
}

void ESS::ssControlOSQTimerInterval_MBHandler(OS *p_os, OSTimer *p_ostimer) {
    qDebug() << "ESS::ssControlOSQTimerInterval_MBHandler - ThreadId: "
             << QThread::currentThreadId();
    QTimer *p_timer = new QTimer(this);
    p_timer->start(OS_TIMER_INTVL_UPDATE);
    connect(p_timer, &QTimer::timeout, this, [=]() {
        if (p_os->m_stat.modbusHanderCnt.cnt_unFinished > OS_STAT_MBH_CNT_UNFINISHED_MAX) {
            do_stopOSQTimerIntvl(p_ostimer);
        }
        else if (p_os->m_stat.modbusHanderCnt.cnt_unFinished < OS_STAT_MBH_CNT_UNFINISHED_MIN) {
            do_startOSQTimerIntvl(p_ostimer);
        }
    });
}

void ESS::ssControlOSQTimerInterval_SerialCommu(OS *p_os, OSTimer *p_ostimer) {
    qDebug() << "ESS::ssControlOSQTimerInterval_SerialCommu - ThreadId: " << QThread::currentThreadId();
    QTimer *p_timer = new QTimer(this);
    p_timer->start(OS_TIMER_INTVL_UPDATE);
    auto request_queue_size = mp_ems->mp_serialCommuCom3->m_requestQueue.size();
    connect(p_timer, &QTimer::timeout, this, [=]() {
        if (request_queue_size > 100) {
            do_stopOSQTimerIntvl(p_ostimer);
        }
        else if (request_queue_size < 10) {
            do_startOSQTimerIntvl(p_ostimer);
        }
    });
}

void ESS::do_startOSQTimerIntvl(OSTimer *p_ostimer) {
    qDebug() << "ESS::do_startOSQTimerIntvl - ThreadId: " << QThread::currentThreadId();
    if (!p_ostimer->m_intvl_01sec.isActive())
        p_ostimer->m_intvl_01sec.start(OS_TIMER_INTVL_01SEC);
    if (!p_ostimer->m_intvl_05sec.isActive())
        p_ostimer->m_intvl_05sec.start(OS_TIMER_INTVL_05SEC);
    if (!p_ostimer->m_intvl_30sec.isActive())
        p_ostimer->m_intvl_30sec.start(OS_TIMER_INTVL_30SEC);
    if (!p_ostimer->m_intvl_02min.isActive())
        p_ostimer->m_intvl_02min.start(OS_TIMER_INTVL_02MIN);
    if (!p_ostimer->m_intvl_10min.isActive())
        p_ostimer->m_intvl_10min.start(OS_TIMER_INTVL_10MIN);
}

void ESS::do_stopOSQTimerIntvl(OSTimer *p_ostimer) {
    qDebug() << "ESS::do_startOSQTimerIntvl - ThreadId: " << QThread::currentThreadId();
    p_ostimer->m_intvl_01sec.stop();
    p_ostimer->m_intvl_05sec.stop();
    // p_ostimer->m_intvl_30sec.stop();
    // p_ostimer->m_intvl_02min.stop();
    // p_ostimer->m_intvl_10min.stop();
}

void ESS::do_emsMBHandlerCom2_01sec() {
    qDebug() << "ESS::do_emsMBHandlerCom2_01sec - ThreadId: " << QThread::currentThreadId();
    ++os[2].m_stat.qtimerCnt.cnt_01sec;
    mp_ems->mp_mbHandlerCom2->sendRequest(mp_meterGrid->m_mbRequestGetDataVAW);
    mp_ems->mp_mbHandlerCom2->sendRequest(mp_meterLoad->m_mbRequestGetDataVAW);
    mp_ems->mp_mbHandlerCom2->sendRequest(mp_meterESS->m_mbRequestGetDataVAW);
    qDebug() << " meterGrid.m_mbReplyGetDataVAW : " << mp_meterGrid->m_mbDataUnitReplyGetDataVAW.values();
    qDebug() << " meterLoad.m_mbReplyGetDataVAW : " << mp_meterLoad->m_mbDataUnitReplyGetDataVAW.values();
    qDebug() << "  meterEss.m_mbReplyGetDataVAW : " << mp_meterESS->m_mbDataUnitReplyGetDataVAW.values();
}

void ESS::do_emsMBHandlerCom2_05sec() {
    qDebug() << "ESS::do_emsMBHandlerCom2_05sec - ThreadId: " << QThread::currentThreadId();
    ++os[2].m_stat.qtimerCnt.cnt_05sec;
    mp_ems->mp_mbHandlerCom2->sendRequest(mp_meterGrid->m_mbRequestGetDataEnergy);
    mp_ems->mp_mbHandlerCom2->sendRequest(mp_meterLoad->m_mbRequestGetDataEnergy);
    mp_ems->mp_mbHandlerCom2->sendRequest(mp_meterESS->m_mbRequestGetDataEnergy);
    qDebug() << " meterGrid.m_mbReplyGetDataEnergy : "
             << mp_meterGrid->m_mbDataUnitReplyGetDataEnergy.values();
    qDebug() << " meterLoad.m_mbReplyGetDataEnergy : "
             << mp_meterLoad->m_mbDataUnitReplyGetDataEnergy.values();
    qDebug() << "  meterEss.m_mbReplyGetDataEnergy : "
             << mp_meterESS->m_mbDataUnitReplyGetDataEnergy.values();
}

void ESS::do_emsMBHandlerCom2_30sec() {}

void ESS::do_emsMBHandlerCom2_02min() {
    qDebug() << "ESS::do_emsMBHandlerCom2_02min - ThreadId: " << QThread::currentThreadId();
    ++os[2].m_stat.qtimerCnt.cnt_02min;
    mp_ems->mp_mbHandlerCom2->sendRequest(mp_meterGrid->m_mbRequestGetParaSys);
    mp_ems->mp_mbHandlerCom2->sendRequest(mp_meterLoad->m_mbRequestGetParaSys);
    mp_ems->mp_mbHandlerCom2->sendRequest(mp_meterESS->m_mbRequestGetParaSys);
    qDebug() << " meterGrid.m_mbReplyGetParaSys : " << mp_meterGrid->m_mbDataUnitReplyGetParaSys.values();
    qDebug() << " meterLoad.m_mbReplyGetParaSys : " << mp_meterLoad->m_mbDataUnitReplyGetParaSys.values();
    qDebug() << "  meterEss.m_mbReplyGetParaSys : " << mp_meterESS->m_mbDataUnitReplyGetParaSys.values();
}

void ESS::do_emsMBHandlerCom2_10min() {
    qDebug() << "ESS::do_emsMBHandlerCom2_10min - ThreadId: " << QThread::currentThreadId();
    ++os[2].m_stat.qtimerCnt.cnt_10min;
    mp_ems->mp_mbHandlerCom2->sendRequest(mp_meterGrid->m_mbRequestGetParaTOUA);
    mp_ems->mp_mbHandlerCom2->sendRequest(mp_meterLoad->m_mbRequestGetParaTOUA);
    mp_ems->mp_mbHandlerCom2->sendRequest(mp_meterESS->m_mbRequestGetParaTOUA);
    qDebug() << " meterGrid.m_mbReplyGetParaTOUA : " << mp_meterGrid->m_mbDataUnitReplyGetParaTouA.values();
    qDebug() << " meterLoad.m_mbReplyGetParaTOUA : " << mp_meterLoad->m_mbDataUnitReplyGetParaTouA.values();
    qDebug() << "  meterEss.m_mbReplyGetParaTOUA : " << mp_meterESS->m_mbDataUnitReplyGetParaTouA.values();
}

void ESS::do_emsSerialCommuCom3_01sec() {
    qDebug() << "ESS::do_emsSerialCommuCom3_01sec - ThreadId: " << QThread::currentThreadId();
    ++os[3].m_stat.qtimerCnt.cnt_01sec;
    mp_ems->mp_serialCommuCom3->sendRequest(mp_ariC->m_serialRequestData01);
    mp_ems->mp_serialCommuCom3->sendRequest(mp_ariC->m_serialRequestData02);
    mp_ems->mp_serialCommuCom3->sendRequest(mp_ariC->m_serialRequestData03);
    qDebug() << " mp_ariC->m_serialRequestData01 : " << mp_ariC->m_byteArrayReplyData01;
    qDebug() << " mp_ariC->m_serialRequestData02 : " << mp_ariC->m_byteArrayReplyData02;
    qDebug() << " mp_ariC->m_serialRequestData03 : " << mp_ariC->m_byteArrayReplyData03;
}
