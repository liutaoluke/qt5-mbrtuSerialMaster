#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCoreApplication>

MainWindow::MainWindow(OS *p_os, QWidget *parent)
    : QMainWindow(parent)
    , mp_os(p_os)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->pushButtonQuit, &QPushButton::clicked, [=]() {
        QCoreApplication::quit();
    });

    // mp_mainWindowCom1 = new MainWindowCom1;
    // connect(ui->pushButtonCom1, &QPushButton::clicked, [=]() {
    //     this->hide();
    //     mp_mainWindowCom1->show();
    // });
    connect(ui->pushButtonCom1, &QPushButton::clicked, this, &MainWindow::do_openMainWindowCom1);

    // mp_mainWindowCom2 = new MainWindowCom2(&(p_os + 1)->m_interval.timer_01sec, (p_os + 1));
    // connect(ui->pushButtonCom2, &QPushButton::clicked, [=]() {
    //     this->hide();
    //     mp_mainWindowCom2->show();
    // });
    connect(ui->pushButtonCom2, &QPushButton::clicked, this, &MainWindow::do_openMainWindowCom2);

    // mp_mainWindowCom3 = new MainWindowCom3(&(p_os + 2)->m_interval.timer_01sec, (p_os + 2), this);
    // connect(ui->pushButtonCom3, &QPushButton::clicked, [=]() {
    //     this->hide();
    //     mp_mainWindowCom3->show();
    // });
    connect(ui->pushButtonCom3, &QPushButton::clicked, this, &MainWindow::do_openMainWindowCom3);

    // connect(mp_mainWindowCom1, &MainWindowCom1::sg_backToMainWindow, this, [=]() {
    //     qDebug() << "!!!!!!MainWindow receive sg_backToMainWindow()!!!!!!";
    //     if (mp_mainWindowCom1) {
    //         qDebug() << " mp_mainWindowCom1 is not equal nullptr";
    //     }
    //     else {
    //         qDebug() << " mp_mainWindowCom1 is equal nullptr!!!";
    //     }

    //     mp_mainWindowCom1->hide();
    //     // mp_mainWindowCom1->deleteLater();
    //     this->show();
    // });

    // connect(mp_mainWindowCom2, &MainWindowCom2::sg_backToMainWindow, this, [=]() {
    //     mp_mainWindowCom2->hide();
    //     // mp_mainWindowCom2->deleteLater();
    //     this->show();
    // });

    // connect(mp_mainWindowCom3, &MainWindowCom3::sg_backToMainWindow, this, [=]() {
    //     mp_mainWindowCom3->hide();
    //     // mp_mainWindowCom3->deleteLater();
    //     this->show();
    // });
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::do_openMainWindowCom1() {
    qDebug() << "!!!!!!MainWindow::do_openMainWindowCom1()!!!!!!";
    mp_mainWindowCom1 = new MainWindowCom1(this);
    this->hide();
    mp_mainWindowCom1->show();

    auto connected_status = connect(mp_mainWindowCom1,
                                    &MainWindowCom1::sg_backToMainWindow,
                                    this,
                                    &MainWindow::do_closeMainWindowCom1);
    qDebug() << "do_closeMainWindowCom1 - Connection status:" << connected_status;
}

void MainWindow::do_closeMainWindowCom1() {
    qDebug() << "!!!!!!start:MainWindow::do_closeMainWindowCom1()!!!!!!";
    mp_mainWindowCom1->hide();
    mp_mainWindowCom1->deleteLater();
    this->show();
    qDebug() << "!!!!!!end:MainWindow::do_closeMainWindowCom1()!!!!!!";
}

void MainWindow::do_openMainWindowCom2() {
    qDebug() << "!!!!!!MainWindow::do_openMainWindowCom2()!!!!!!";
    mp_mainWindowCom2 = new MainWindowCom2(&(mp_os + 1)->m_interval.timer_01sec, (mp_os + 1));
    this->hide();
    mp_mainWindowCom2->show();

    auto connected_status = connect(mp_mainWindowCom2,
                                    &MainWindowCom2::sg_backToMainWindow,
                                    this,
                                    &MainWindow::do_closeMainWindowCom2);
    qDebug() << "do_closeMainWindowCom2 - Connection status:" << connected_status;
}

void MainWindow::do_closeMainWindowCom2() {
    qDebug() << "!!!!!!start:MainWindow::do_closeMainWindowCom2()!!!!!!";
    mp_mainWindowCom2->hide();
    mp_mainWindowCom2->deleteLater();
    this->show();
    qDebug() << "!!!!!!end:MainWindow::do_closeMainWindowCom2()!!!!!!";
}

void MainWindow::do_openMainWindowCom3() {
    qDebug() << "!!!!!!MainWindow::do_openMainWindowCom3()!!!!!!";
    mp_mainWindowCom3 = new MainWindowCom3(&(mp_os + 2)->m_interval.timer_01sec, (mp_os + 2));
    this->hide();
    mp_mainWindowCom3->show();

    auto connected_status = connect(mp_mainWindowCom3,
                                    &MainWindowCom3::sg_backToMainWindow,
                                    this,
                                    &MainWindow::do_closeMainWindowCom3);
    qDebug() << "do_closeMainWindowCom3 - Connection status:" << connected_status;
}

void MainWindow::do_closeMainWindowCom3() {
    qDebug() << "!!!!!!start:MainWindow::do_closeMainWindowCom3()!!!!!!";
    mp_mainWindowCom3->hide();
    mp_mainWindowCom3->deleteLater();
    this->show();
    qDebug() << "!!!!!!end:MainWindow::do_closeMainWindowCom3()!!!!!!";
}
