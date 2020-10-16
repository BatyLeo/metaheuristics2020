include(../default.pri)

CONFIG   -= app_bundle
#QT += serialport

TEMPLATE = lib

TARGET = myapp

QT += widgets

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += \
    algorithms/heuristics.cpp \
    algorithms/metaheuristics.cpp \
    data_structures/data_set.cpp \
    data_structures/solution.cpp \
    tools/parsing.cpp

HEADERS += \
    algorithms/heuristics.h \
    algorithms/metaheuristics.h \
    data_structures/data_set.h \
    data_structures/solution.h \
    tools/parsing.h

DISTFILES += \
    algorithms/README
