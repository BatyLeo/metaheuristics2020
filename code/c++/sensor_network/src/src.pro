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
    tools/parsing.cpp \
    interface/data_models.cpp \
    interface/sidebar.cpp \
    interface/solution_view.cpp \
    interface/central_widget.cpp \
    interface/mainwindow.cpp \
    algorithms/basic_algorithms.cpp

HEADERS += \
    algorithms/heuristics.h \
    algorithms/metaheuristics.h \
    data_structures/data_set.h \
    data_structures/solution.h \
    tools/parsing.h \
    interface/mainwindow.h \
    interface/centralwidget.h \
    interface/sidebar.h \
    interface/data_models.h \
    interface/solution_view.h \
    algorithms/basic_algorithms.h

DISTFILES += \
    algorithms/README
