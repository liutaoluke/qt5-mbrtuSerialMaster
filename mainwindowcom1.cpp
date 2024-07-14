#include "mainwindowcom1.h"
#include "ui_mainwindowcom1.h"

MainWindowCom1::MainWindowCom1(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowCom1) {
    ui->setupUi(this);
    qDebug() << "constructor - MainWindowCom1::MainWindowCom1(QWidget *parent)";

    connect(ui->pushButtonBack, &QPushButton::clicked, this, [=]() {
        qDebug() << "****** MainWindowCom1 before emit sg_backToMainWindow() ******";
        emit sg_backToMainWindow();
        qDebug() << "****** MainWindowCom1 after emit sg_backToMainWindow() ******";
    });
}

MainWindowCom1::~MainWindowCom1() {
    qDebug() << "destructor - MainWindowCom1::~MainWindowCom1()";
    delete ui;
}
