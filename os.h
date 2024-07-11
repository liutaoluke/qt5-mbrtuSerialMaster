#pragma once

#include <QObject>

struct Statistics {
    quint64 cntQTimer_500ms = 0;
    quint64 cntQTimer_01sec = 0;
    quint64 cntQTimer_05sec = 0;
    quint64 cntQTimer_30sec = 0;
    quint64 cntQTimer_02min = 0;
    quint64 cntQTimer_10min = 0;
};

class OS : public QObject {
    Q_OBJECT
public:
    explicit OS(QObject *parent = nullptr);

signals:

public:
    Statistics stat;
};
