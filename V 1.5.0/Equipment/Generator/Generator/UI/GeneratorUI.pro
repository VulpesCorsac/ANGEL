#-------------------------------------------------
#
# Project created by QtCreator 2016-09-12T09:09:05
#
#-------------------------------------------------

QT       += core gui serialport testlib
CONFIG   += c++11

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

QT.testlib.CONFIG -= console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3

TARGET = GeneratorUI
TEMPLATE = app

SOURCES += main.cpp\
        GeneratorUI.cpp \
    ../Generator.cpp \
    ../../DS335/DS335.cpp \
    ../../DS345/DS345.cpp \
    ../../../CommonFunctionsForSRS/SRS.cpp \
    ../../../../Functions/AllFunctions.cpp

HEADERS  += GeneratorUI.h \
    ../Generator.h \
    ../../DS335/DS335.h \
    ../../DS345/DS345.h \
    ../../../CommonFunctionsForSRS/SRS.h \
    ../../../../Functions/AllFunctions.h

FORMS    += GeneratorUI.ui
