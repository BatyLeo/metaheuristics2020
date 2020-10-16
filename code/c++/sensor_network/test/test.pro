TEMPLATE = app
 
include(../default.pri)

QMAKE_LFLAGS_RPATH =
QMAKE_LFLAGS += -Wl,-rpath,"'\$$ORIGIN/../src'"
 
CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG += qtc_runnable
CONFIG -= qt
#QT += serialport
 
SOURCES += main.cpp
 
LIBS += -L../src -lmyapp
