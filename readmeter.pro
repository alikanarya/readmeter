QT += core
QT -= gui

CONFIG += c++11

TARGET = readmeter
target.files = readmeter
target.path=/root/app

INSTALLS += target

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp
