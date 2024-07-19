#include "mainwindowcom1.h"
#include "ui_mainwindowcom1.h"

MainWindowCom1::MainWindowCom1(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowCom1) {
    ui->setupUi(this);
    qInfo() << "constructor - MainWindowCom1::MainWindowCom1(QWidget *parent)";

    connect(ui->pushButtonBack, &QPushButton::clicked, this, [=]() {
        qInfo() << "****** MainWindowCom1 before emit sg_backToMainWindow() ******";
        emit sg_backToMainWindow();
        qInfo() << "****** MainWindowCom1 after emit sg_backToMainWindow() ******";
    });
}

MainWindowCom1::~MainWindowCom1() {
    qWarning() << "destructor - MainWindowCom1::~MainWindowCom1()";
    delete ui;
}
