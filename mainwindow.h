#pragma once
#include "os.h"

#include <QMainWindow>

#include "mainwindowcom1.h"
#include "mainwindowcom2.h"
#include "mainwindowcom3.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void do_openMainWindowCom1();
    void do_closeMainWindowCom1();
    void do_openMainWindowCom2();
    void do_closeMainWindowCom2();
    void do_openMainWindowCom3();
    void do_closeMainWindowCom3();

private:
    Ui::MainWindow *ui;

private:
    MainWindowCom1 *mp_mainWindowCom1 = nullptr;
    MainWindowCom2 *mp_mainWindowCom2 = nullptr;
    MainWindowCom3 *mp_mainWindowCom3 = nullptr;
};
