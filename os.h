#pragma once
#include "def.h"

#include <QObject>

//Statistics_stDT
enum OS_Statistics_ModbusHanderCnt_enDT {
    OS_STAT_MBH_CNT_UNFINISHED_MIN = 10,
    OS_STAT_MBH_CNT_UNFINISHED_MAX = 100
};
struct OS_Statistics_ModbusHanderCnt_stDT {
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
            percent_Finished = static_cast<double>(cnt_processNextRequest_Finished) * 100
                               / cnt_sendRequest;
        }
    }
};

struct OS_Statistics_QTimerCnt_stDT {
    quint64 cnt_500ms = 0;
    quint64 cnt_01sec = 0;
    quint64 cnt_05sec = 0;
    quint64 cnt_30sec = 0;
    quint64 cnt_02min = 0;
    quint64 cnt_10min = 0;
};

struct OS_Statistics_stDT {
    OS_Statistics_QTimerCnt_stDT qtimerCnt;
    OS_Statistics_ModbusHanderCnt_stDT modbusHanderCnt;
};

class OS : public QObject {
    Q_OBJECT
public:
    explicit OS(QObject *parent = nullptr);
    virtual ~OS();
signals:

public:
    OS_Statistics_stDT m_stat;
};

//declaration
extern OS os[5];
