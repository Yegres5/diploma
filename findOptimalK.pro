#-------------------------------------------------
#
# Project created by QtCreator 2019-02-05T10:42:23
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = findOptimalK
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    mainwindow/lainitialframe.ui \
    mainwindow/modelingframeparameters.ui \
    mainwindow/rocketinitialframe.ui \
    AcceptableKDialog/resultframe.ui \
    AcceptableKDialog/headeritem.ui

HEADERS += \
    AcceptableKDialog/acceptablek.h \
    mainwindow/initialmodelingframe.h \
    mainwindow/lainitialframe.h \
    mainwindow/mainwindow.h \
    mainwindow/modelingframeparameters.h \
    mainwindow/rocketinitialframe.h \
    AcceptableKDialog/resultframe.h \
    ModelClasses/model.h \
    ModelClasses/simulator.h \
    ModelClasses/FlyObjects/la.h \
    ModelClasses/FlyObjects/rocket.h \
    ModelClasses/FlyObjects/UsefulClassesForLA/angle.h \
    AcceptableKDialog/headeritem.h \
    AcceptableKDialog/tabledelegate.h \
    AcceptableKDialog/tabledrawingdata.h

SOURCES += \
    AcceptableKDialog/acceptablek.cpp \
    mainwindow/initialmodelingframe.cpp \
    mainwindow/lainitialframe.cpp \
    mainwindow/mainwindow.cpp \
    mainwindow/modelingframeparameters.cpp \
    mainwindow/rocketinitialframe.cpp \
    main.cpp \
    AcceptableKDialog/resultframe.cpp \
    ModelClasses/model.cpp \
    ModelClasses/simulator.cpp \
    ModelClasses/FlyObjects/la.cpp \
    ModelClasses/FlyObjects/rocket.cpp \
    ModelClasses/FlyObjects/UsefulClassesForLA/angle.cpp \
    AcceptableKDialog/headeritem.cpp \
    AcceptableKDialog/tabledelegate.cpp \
    AcceptableKDialog/tabledrawingdata.cpp




