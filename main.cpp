#include "mainwindow.h"

#include <QApplication>

#include "ess.h"
#include "os.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    OS os[3];
    ESS ess(os);

    MainWindow w(os);
    w.show();

    return a.exec();
}
