#pragma once

#include <QObject>
#include <QTimer>

//QTimerInterval
enum OS_QTimerInterval_enDT {
    OS_QTIMER_INTERVAL_UPDATE = 1000,
    OS_QTIMER_INTERVAL_01SEC = 500,
    OS_QTIMER_INTERVAL_05SEC = 1000,
    OS_QTIMER_INTERVAL_30SEC = 30000,
    OS_QTIMER_INTERVAL_02MIN = 12000,
    OS_QTIMER_INTERVAL_10MIN = 60000,
};
struct QTimerInterval_stDT {
    QTimer timer_01sec;
    QTimer timer_05sec;
    QTimer timer_30sec;
    QTimer timer_02min;
    QTimer timer_10min;
    // QTimer timer_1hour;
    // QTimer timer_6hour;
    // QTimer timer_01day;
    // QTimer timer_1week;
    // QTimer timer_01mon;
};
//Statistics_stDT
enum OS_Statistics_ModbusHander_enDT {
    OS_STAT_MBH_CNT_UNFINISHED_MIN = 10,
    OS_STAT_MBH_CNT_UNFINISHED_MAX = 100
};
struct Statistics_ModbusHander_stDT {
    quint64 cnt_sendRequest = 0;
    quint64 cnt_processNextRequest = 0;
    quint64 cnt_processNextRequest_Read = 0;
    quint64 cnt_processNextRequest_Write = 0;
    quint64 cnt_processNextRequest_RW = 0;
    quint64 cnt_processNextRequest_NONE = 0;
    quint64 cnt_processNextRequest_NULL = 0;
    quint64 cnt_processNextRequest_Finished = 0;
    quint64 cnt_processNextRequest_Error = 0;
    quint64 cnt_onErrorOccurred = 0;
    quint64 cnt_onStateChanged_ConnectedState = 0;
    quint64 cnt_onStateChanged_UnconnectedState = 0;

    double percent_Finished = 0; //Finished/send*100%
    quint64 cnt_unFinished = 0;

    void calc_percent_Finished() {
        if (cnt_sendRequest != 0) {
            percent_Finished = static_cast<double>(cnt_processNextRequest_Finished) / cnt_sendRequest;
        }
    }
};

struct Statistics_QTimer_stDT {
    quint64 cnt_500ms = 0;
    quint64 cnt_01sec = 0;
    quint64 cnt_05sec = 0;
    quint64 cnt_30sec = 0;
    quint64 cnt_02min = 0;
    quint64 cnt_10min = 0;
};

struct Statistics_stDT {
    Statistics_QTimer_stDT qtimer;
    Statistics_ModbusHander_stDT modbusHander;
};

class OS : public QObject {
    Q_OBJECT
public:
    explicit OS(QObject *parent = nullptr);

signals:

public:
    Statistics_stDT m_stat;
    QTimerInterval_stDT m_interval;
};
