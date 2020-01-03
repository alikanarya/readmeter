QT += core
#QT -= gui
QT += network
QT += sql

CONFIG += c++11

TARGET = readmeter
target.files = readmeter
target.path=/root/app

INSTALLS += target

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    netops.cpp \
    datathread.cpp

HEADERS += \
    netops.h \
    datathread.h
