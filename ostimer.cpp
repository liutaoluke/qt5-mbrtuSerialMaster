#include "ostimer.h"

#include <QDebug>
#include <QThread>

OSTimer::OSTimer(QObject *parent)
    : QObject{parent} {
    m_intvl_01sec.start(OS_TIMER_INTVL_01SEC);
    m_intvl_05sec.start(OS_TIMER_INTVL_05SEC);
    m_intvl_30sec.start(OS_TIMER_INTVL_30SEC);
    m_intvl_02min.start(OS_TIMER_INTVL_02MIN);
    m_intvl_10min.start(OS_TIMER_INTVL_10MIN);
}

OSTimer::~OSTimer() {
    qCritical() << "destructor - OSTimer::~OSTimer()!!!";
    qDebug() << "destructor - OSTimer::~OSTimer() - ThreadId: " << QThread::currentThreadId();
}
