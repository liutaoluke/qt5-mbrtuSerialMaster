#include "os.h"

OS::OS(QObject *parent)
    : QObject{parent} {
    //QTimerInvertal
    m_interval.timer_01sec.start(OS_QTIMER_INTERVAL_01SEC);
    m_interval.timer_05sec.start(OS_QTIMER_INTERVAL_05SEC);
    m_interval.timer_30sec.start(OS_QTIMER_INTERVAL_30SEC);
    m_interval.timer_02min.start(OS_QTIMER_INTERVAL_02MIN);
    m_interval.timer_10min.start(OS_QTIMER_INTERVAL_10MIN);
}
