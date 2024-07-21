#include "mainwindowdisplay.h"
#include "ui_mainwindowdisplay.h"

#include <QDebug>
#include <QListWidget>
#include <QTimer>

MainWindowDisplay::MainWindowDisplay(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowDisplay) {
    ui->setupUi(this);

    connect(ui->pushButtonBack, &QPushButton::clicked, this, [=]() {
        qInfo() << "****** MainWindowDisplay before emit sg_backToMainWindow() ******";
        emit sg_backToMainWindow();
        qInfo() << "****** MainWindowDisplay after emit sg_backToMainWindow() ******";
    });

    QTimer *p_timer_ui = new QTimer(this);
    p_timer_ui->setInterval(UI_TIMER_INTVL_UPDATE);
    p_timer_ui->start();

    OS *p_os = &os[2];
    connect(p_timer_ui, &QTimer::timeout, this, [=]() {
        m_stringList.clear();

        m_stringList << QString("**************************** ");
        m_stringList << QString("modbusHander - stat - com2 : ");
        m_stringList << QString("**************************** ");

        m_stringList << QString("cnt_sendRequest : %1")
                            .arg(p_os->m_stat.modbusHanderCnt.cnt_sendRequest);
        // m_stringList.append("cnt_processNextRequest : "
        //                     + QString::number(p_os->m_stat.modbusHanderCnt.cnt_processNextRequest));
        m_stringList << QString("cnt_processNextRequest : %1")
                            .arg(p_os->m_stat.modbusHanderCnt.cnt_processNextRequest);
        m_stringList << QString("cnt_processNextRequest_Read : %1")
                            .arg(p_os->m_stat.modbusHanderCnt.cnt_processNextRequest_Read);
        m_stringList << QString("cnt_processNextRequest_Write : %1")
                            .arg(p_os->m_stat.modbusHanderCnt.cnt_processNextRequest_Write);
        m_stringList << QString("cnt_processNextRequest_RW : %1")
                            .arg(p_os->m_stat.modbusHanderCnt.cnt_processNextRequest_RW);
        m_stringList << QString("cnt_processNextRequest_NONE : %1")
                            .arg(p_os->m_stat.modbusHanderCnt.cnt_processNextRequest_NONE);
        // m_stringList.append(
        //     "cnt_processNextRequest_Finished : "
        //     + QString::number(p_os->m_stat.modbusHanderCnt.cnt_processNextRequest_Finished));
        m_stringList << QString("cnt_processNextRequest_Finished : %1")
                            .arg(p_os->m_stat.modbusHanderCnt.cnt_processNextRequest_Finished);
        m_stringList << QString("cnt_processNextRequest_Error : %1")
                            .arg(p_os->m_stat.modbusHanderCnt.cnt_processNextRequest_Error);
        m_stringList << QString("cnt_processNextRequest_NULL : %1")
                            .arg(p_os->m_stat.modbusHanderCnt.cnt_processNextRequest_NULL);

        // m_stringList.append("cnt_unFinished : "
        //                     + QString::number(p_os->m_stat.modbusHanderCnt.cnt_unFinished));
        m_stringList << QString("cnt_unFinished : %1")
                            .arg(p_os->m_stat.modbusHanderCnt.cnt_unFinished);
        m_stringList << QString("percent_Finished% : %1")
                            .arg(p_os->m_stat.modbusHanderCnt.percent_Finished);
        m_stringList << QString("cnt_onErrorOccurred : %1")
                            .arg(p_os->m_stat.modbusHanderCnt.cnt_onErrorOccurred);
        m_stringList << QString("cnt_onStateChanged_ConnectedState : %1")
                            .arg(p_os->m_stat.modbusHanderCnt.cnt_onStateChanged_ConnectedState);
        m_stringList << QString("cnt_onStateChanged_UnconnectedState : %1")
                            .arg(p_os->m_stat.modbusHanderCnt.cnt_onStateChanged_UnconnectedState);

        ui->listWidgetDisplay->clear();
        ui->listWidgetDisplay->addItems(m_stringList);
    });
}

MainWindowDisplay::~MainWindowDisplay() {
    qCritical() << "destructor - MainWindowDisplay::~MainWindowDisplay()";
    delete ui;
}
