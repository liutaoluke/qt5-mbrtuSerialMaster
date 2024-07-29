#pragma once

#include <QObject>

class PowerBus : public QObject {
    Q_OBJECT
public:
    explicit PowerBus(QObject *parent = nullptr);

signals:
};
