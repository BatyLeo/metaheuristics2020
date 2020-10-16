include(../default.pri)

QT += widgets

QMAKE_CXXFLAGS += -std=c++0x

QMAKE_LFLAGS_RPATH =
QMAKE_LFLAGS = -Wl,-rpath,"'\$$ORIGIN/../src'",--enable-new-dtags
 
CONFIG -= app_bundle
CONFIG += qtc_runnable
#QT += serialport
 
TEMPLATE = app
 
SOURCES += main.cpp
 
LIBS += -L../src -lmyapp

