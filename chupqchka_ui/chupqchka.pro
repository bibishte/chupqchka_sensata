#-------------------------------------------------
#
# Project created by QtCreator 2017-12-07T17:55:37
#
#-------------------------------------------------

QT       += core gui
 QT += serialport
QT += core
CONFIG += c++11
LIBS += -lQt5SerialPort

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chupqchka
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    state_machine.cpp

HEADERS  += mainwindow.h \
    state_machine.h

FORMS    += mainwindow.ui
