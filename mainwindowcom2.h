#pragma once
#include "def.h"

#include <QMainWindow>
#include "os.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindowCom2;
}
QT_END_NAMESPACE

class MainWindowCom2 : public QMainWindow {
    Q_OBJECT

public:
    MainWindowCom2(QTimer *p_timer, OS *p_os, QWidget *parent = nullptr);
    ~MainWindowCom2();

signals:
    void sg_backToMainWindow();

private:
    Ui::MainWindowCom2 *ui;
};
