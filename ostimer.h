#pragma once

#include <QObject>
#include <QTimer>

//QTimerINTVL
enum OS_Timer_Intvl_enDT {
    OS_TIMER_INTVL_UPDATE = 1000,

    OS_TIMER_INTVL_01SEC = 500,
    OS_TIMER_INTVL_05SEC = 1000,
    OS_TIMER_INTVL_30SEC = 30000,
    OS_TIMER_INTVL_02MIN = 120000,
    OS_TIMER_INTVL_10MIN = 600000,
};

class OSTimer : public QObject {
    Q_OBJECT
public:
    explicit OSTimer(QObject *parent = nullptr);
    virtual ~OSTimer();

signals:

public:
    QTimer m_intvl_01sec;
    QTimer m_intvl_05sec;
    QTimer m_intvl_30sec;
    QTimer m_intvl_02min;
    QTimer m_intvl_10min;
    // QTimer m_intvl_1hour;
    // QTimer m_intvl_6hour;
    // QTimer m_intvl_01day;
    // QTimer m_intvl_1week;
    // QTimer m_intvl_01mon;
};
