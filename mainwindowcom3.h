#pragma once
#include "def.h"

#include <QMainWindow>
#include "os.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindowCom3;
}
QT_END_NAMESPACE

class MainWindowCom3 : public QMainWindow {
    Q_OBJECT

public:
    MainWindowCom3(QWidget *parent = nullptr);
    ~MainWindowCom3();

signals:
    void sg_backToMainWindow();

private:
    Ui::MainWindowCom3 *ui;
};
