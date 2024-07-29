#include "mainwindowcom2.h"
#include "ui_mainwindowcom2.h"

#include <QDebug>
#include <QLabel>
#include <QString>
#include <QThread>
#include <QTimer>

MainWindowCom2::MainWindowCom2(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowCom2) {
    ui->setupUi(this);
    qInfo() << "constructor - MainWindowCom2::MainWindowCom2(QWidget *parent)";

    connect(ui->pushButtonBack, &QPushButton::clicked, this, [=]() {
        qInfo() << "****** MainWindowCom2 before emit sg_backToMainWindow() ******";
        emit sg_backToMainWindow();
        qInfo() << "****** MainWindowCom2 after emit sg_backToMainWindow() ******";
    });

    // QTimer *p_timer = &os[2].m_qtimerIntvl.intvl_01sec;
    // // connect(ui->pushButtonStart, &QPushButton::clicked, p_timer, &QTimer::start);
    // connect(ui->pushButtonStart, &QPushButton::clicked, p_timer, [p_timer]() {
    //     p_timer->start();
    // });
    // connect(ui->pushButtonStop, &QPushButton::clicked, p_timer, &QTimer::stop);

    // connect(ui->pushButtonIncrease, &QPushButton::clicked, p_timer, [p_timer]() {
    //     auto interval = p_timer->interval();
    //     interval += 100;
    //     if (interval >= 2000) {
    //         interval = 2000;
    //     }
    //     p_timer->setInterval(interval);
    // });

    // connect(ui->pushButtonDecrease, &QPushButton::clicked, p_timer, [p_timer]() {
    //     auto interval = p_timer->interval();
    //     interval -= 100;
    //     if (interval <= 500) {
    //         interval = 500;
    //     }
    //     p_timer->setInterval(interval);
    // });

    // QTimer::singleShot(3000, p_timer, [p_timer]() {
    //     qDebug() << "Stopping p_timer...";
    //     p_timer->stop();

    //     // 再次启动定时器，间隔为1000毫秒
    //     QTimer::singleShot(2000, p_timer, [p_timer]() {
    //         qDebug() << "Restarting p_timer...";
    //         p_timer->start(1000);
    //     });
    // });

    QTimer *p_timer_ui = new QTimer(this);
    p_timer_ui->setInterval(UI_TIMER_INTVL_UPDATE);
    p_timer_ui->start();

    OS *p_os = &os[2];
    connect(p_timer_ui, &QTimer::timeout, this, [=]() {
        qDebug() << "MainWindowCom2::MainWindowCom2 - ThreadId: " << QThread::currentThreadId();

        ui->labelStatCntQTimer_500ms->setText(QString::number(p_os->m_stat.qtimerCnt.cnt_500ms));
        qDebug() << "p_os->stat.cntQTimer_500ms : " << p_os->m_stat.qtimerCnt.cnt_500ms;

        ui->labelStatCntQTimer_01sec->setText(QString::number(p_os->m_stat.qtimerCnt.cnt_01sec));
        qDebug() << "p_os->stat.cntQTimer_01sec : " << p_os->m_stat.qtimerCnt.cnt_01sec;

        ui->labelStatCntQTimer_05sec->setText(QString::number(p_os->m_stat.qtimerCnt.cnt_05sec));
        qDebug() << "p_os->stat.cntQTimer_05sec : " << p_os->m_stat.qtimerCnt.cnt_05sec;

        ui->labelStatCntQTimer_02min->setText(QString::number(p_os->m_stat.qtimerCnt.cnt_02min));
        qDebug() << "p_os->stat.cntQTimer_02min : " << p_os->m_stat.qtimerCnt.cnt_02min;

        ui->labelStatCntQTimer_10min->setText(QString::number(p_os->m_stat.qtimerCnt.cnt_10min));
        qDebug() << "p_os->stat.cntQTimer_10min : " << p_os->m_stat.qtimerCnt.cnt_10min;

        ui->labelStatMBHandlercntsendRequest->setText(
            QString::number(p_os->m_stat.modbusHanderCnt.cnt_sendRequest));
        ui->labelStatMBHandlercntprocessNextRequest->setText(
            QString::number(p_os->m_stat.modbusHanderCnt.cnt_processNextRequest));
        ui->labelStatMBHandlercntprocessNextRequestRead->setText(
            QString::number(p_os->m_stat.modbusHanderCnt.cnt_processNextRequest_Read));
        ui->labelStatMBHandlercntprocessNextRequestWrite->setText(
            QString::number(p_os->m_stat.modbusHanderCnt.cnt_processNextRequest_Write));
        ui->labelStatMBHandlercntprocessNextRequestRW->setText(
            QString::number(p_os->m_stat.modbusHanderCnt.cnt_processNextRequest_RW));
        ui->labelStatMBHandlercntprocessNextRequestNone->setText(
            QString::number(p_os->m_stat.modbusHanderCnt.cnt_processNextRequest_NONE));
        ui->labelStatMBHandlercntprocessNextRequestNull->setText(
            QString::number(p_os->m_stat.modbusHanderCnt.cnt_processNextRequest_NULL));
        ui->labelStatMBHandlercntprocessNextRequestFinished->setText(
            QString::number(p_os->m_stat.modbusHanderCnt.cnt_processNextRequest_Finished));
        ui->labelStatMBHandlercntprocessNextRequestError->setText(
            QString::number(p_os->m_stat.modbusHanderCnt.cnt_processNextRequest_Error));
        ui->labelStatMBHandlercntonErrorOccurred->setText(
            QString::number(p_os->m_stat.modbusHanderCnt.cnt_onErrorOccurred));
        ui->labelStatMBHandlercntonStateChanged_ConnectedState->setText(
            QString::number(p_os->m_stat.modbusHanderCnt.cnt_onStateChanged_ConnectedState));
        ui->labelStatMBHandlercntonStateChanged_UnconnectedState->setText(
            QString::number(p_os->m_stat.modbusHanderCnt.cnt_onStateChanged_UnconnectedState));

        auto percent_finished = p_os->m_stat.modbusHanderCnt.percent_Finished;
        ui->labelStatMBHandlerpercentFinished->setNum(static_cast<int>(percent_finished));
        ui->labelStatMBHandlercntunFinished->setText(
            QString::number(p_os->m_stat.modbusHanderCnt.cnt_unFinished));

        // ui->labelTimerInterval->setNum(p_timer->interval());
    });
}

MainWindowCom2::~MainWindowCom2() {
    qCritical() << "destructor - MainWindowCom2::~MainWindowCom2()";
    delete ui;
}
