#include "os.h"

#include <QDebug>
#include <QThread>

OS::OS(QObject *parent)
    : QObject{parent} {}

OS::~OS() {
    qCritical() << "destructor - OS::~OS()!!!";
    qDebug() << "destructor - OS::~OS - ThreadId: " << QThread::currentThreadId();
}
