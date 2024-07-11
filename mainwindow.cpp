#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QLabel>
#include <QString>
#include <QTimer>

MainWindow::MainWindow(OS *p_os, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QTimer *p_timer = new QTimer;
    p_timer->setInterval(500);
    p_timer->start();

    connect(p_timer, &QTimer::timeout, this, [=]() {
        ui->labelStatCntQTimer_500ms->setText(QString::number(p_os->stat.cntQTimer_500ms));
        qDebug() << "p_os->stat.cntQTimer_500ms : " << p_os->stat.cntQTimer_500ms;

        ui->labelStatCntQTimer_01sec->setText(QString::number(p_os->stat.cntQTimer_01sec));
        qDebug() << "p_os->stat.cntQTimer_01sec : " << p_os->stat.cntQTimer_01sec;

        ui->labelStatCntQTimer_05sec->setText(QString::number(p_os->stat.cntQTimer_05sec));
        qDebug() << "p_os->stat.cntQTimer_05sec : " << p_os->stat.cntQTimer_05sec;

        ui->labelStatCntQTimer_02min->setText(QString::number(p_os->stat.cntQTimer_02min));
        qDebug() << "p_os->stat.cntQTimer_02min : " << p_os->stat.cntQTimer_02min;

        ui->labelStatCntQTimer_10min->setText(QString::number(p_os->stat.cntQTimer_10min));
        qDebug() << "p_os->stat.cntQTimer_10min : " << p_os->stat.cntQTimer_10min;
    });
}

MainWindow::~MainWindow() {
    delete ui;
}
