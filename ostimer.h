#pragma once
#include "os.h"

#include <QObject>
#include <QTimer>

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
