#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCoreApplication>
#include <QThread>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->pushButtonQuit, &QPushButton::clicked, [=]() {
        // Set background color to black
        this->setStyleSheet("background-color: black;");

        // Switch to full screen mode
        this->showFullScreen();

        // Update the window to apply the style and mode changes
        this->update();

        // // Process any pending events to ensure the UI updates before hiding
        // QCoreApplication::processEvents();

        // Use QTimer to delay the quit operation, allowing the UI to update
        QTimer::singleShot(5000, this, [=]() {
            // Hide the window
            this->hide();

            // Quit the application
            QCoreApplication::quit();
        });
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

    connect(ui->pushButtonDisplay,
            &QPushButton::clicked,
            this,
            &MainWindow::do_openMainWindowDisplay);
}

MainWindow::~MainWindow() {
    qCritical() << "destructor - MainWindow::~MainWindow()!!!";
    qDebug() << "destructor - MainWindow::~MainWindow - ThreadId: " << QThread::currentThreadId();
    delete ui;
}

void MainWindow::do_openMainWindowCom1() {
    qInfo() << "!!!!!!MainWindow::do_openMainWindowCom1()!!!!!!";
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
    qInfo() << "!!!!!!start:MainWindow::do_closeMainWindowCom1()!!!!!!";
    mp_mainWindowCom1->hide();
    mp_mainWindowCom1->deleteLater();
    this->show();
    qInfo() << "!!!!!!end:MainWindow::do_closeMainWindowCom1()!!!!!!";
}

void MainWindow::do_openMainWindowCom2() {
    qInfo() << "!!!!!!MainWindow::do_openMainWindowCom2()!!!!!!";
    mp_mainWindowCom2 = new MainWindowCom2(this);
    this->hide();
    mp_mainWindowCom2->show();

    auto connected_status = connect(mp_mainWindowCom2,
                                    &MainWindowCom2::sg_backToMainWindow,
                                    this,
                                    &MainWindow::do_closeMainWindowCom2);
    qDebug() << "do_closeMainWindowCom2 - Connection status:" << connected_status;
}

void MainWindow::do_closeMainWindowCom2() {
    qInfo() << "!!!!!!start:MainWindow::do_closeMainWindowCom2()!!!!!!";
    mp_mainWindowCom2->hide();
    mp_mainWindowCom2->deleteLater();
    this->show();
    qInfo() << "!!!!!!end:MainWindow::do_closeMainWindowCom2()!!!!!!";
}

void MainWindow::do_openMainWindowCom3() {
    qInfo() << "!!!!!!MainWindow::do_openMainWindowCom3()!!!!!!";
    mp_mainWindowCom3 = new MainWindowCom3(this);
    this->hide();
    mp_mainWindowCom3->show();

    auto connected_status = connect(mp_mainWindowCom3,
                                    &MainWindowCom3::sg_backToMainWindow,
                                    this,
                                    &MainWindow::do_closeMainWindowCom3);
    qDebug() << "do_closeMainWindowCom3 - Connection status:" << connected_status;
}

void MainWindow::do_closeMainWindowCom3() {
    qInfo() << "!!!!!!start:MainWindow::do_closeMainWindowCom3()!!!!!!";
    mp_mainWindowCom3->hide();
    mp_mainWindowCom3->deleteLater();
    this->show();
    qInfo() << "!!!!!!end:MainWindow::do_closeMainWindowCom3()!!!!!!";
}

void MainWindow::do_openMainWindowDisplay() {
    qInfo() << "!!!!!!MainWindow::do_openMainWindowDisplay()!!!!!!";
    mp_mainWindowDisplay = new MainWindowDisplay(this);
    this->hide();
    mp_mainWindowDisplay->show();

    auto connected_status = connect(mp_mainWindowDisplay,
                                    &MainWindowDisplay::sg_backToMainWindow,
                                    this,
                                    &MainWindow::do_closeMainWindowDisplay);
    qDebug() << "do_openMainWindowDisplay - Connection status:" << connected_status;
}

void MainWindow::do_closeMainWindowDisplay() {
    qInfo() << "!!!!!!start:MainWindow::do_closeMainWindowDisplay()!!!!!!";
    mp_mainWindowDisplay->hide();
    mp_mainWindowDisplay->deleteLater();
    this->show();
    qInfo() << "!!!!!!end:MainWindow::do_closeMainWindowDisplay()!!!!!!";
}
