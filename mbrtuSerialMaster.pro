QT       += core gui
QT       += serialport serialbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ess.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindowcom1.cpp \
    mainwindowcom2.cpp \
    mainwindowcom3.cpp \
    meter.cpp \
    modbushandler.cpp \
    modbusrequest.cpp \
    os.cpp \
    serialcommu.cpp \
    serialrequest.cpp

HEADERS += \
    ess.h \
    mainwindow.h \
    mainwindowcom1.h \
    mainwindowcom2.h \
    mainwindowcom3.h \
    meter.h \
    modbushandler.h \
    modbusrequest.h \
    os.h \
    serialcommu.h \
    serialrequest.h

FORMS += \
    mainwindow.ui \
    mainwindowcom1.ui \
    mainwindowcom2.ui \
    mainwindowcom3.ui

# Default rules for deployment.
# qnx: target.path = /tmp/$${TARGET}/bin
# else: unix:!android: target.path = /opt/$${TARGET}/bin
# !isEmpty(target.path): INSTALLS += target

target.path = /root
INSTALLS += target
