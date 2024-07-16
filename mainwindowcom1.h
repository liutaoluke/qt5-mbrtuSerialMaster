#pragma once
#include "def.h"

#include <QDebug>
#include <QMainWindow>

namespace Ui {
class MainWindowCom1;
}

class MainWindowCom1 : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindowCom1(QWidget *parent = nullptr);
    ~MainWindowCom1();

signals:
    void sg_backToMainWindow();

private:
    Ui::MainWindowCom1 *ui;
};
