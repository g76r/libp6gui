# Copyright 2014 Hallowyn and others.

QT += widgets

TARGET = h6ncsu
TEMPLATE = lib

DEFINES += LIBH6NCSU_LIBRARY

QMAKE_CXXFLAGS += -Wextra
unix:debug:QMAKE_CXXFLAGS += -ggdb
unix {
  OBJECTS_DIR = ../build-h6ncsu-unix/obj
  RCC_DIR = ../build-h6ncsu-unix/rcc
  MOC_DIR = ../build-h6ncsu-unix/moc
}

SOURCES += \
    widget/dynamicstackedwidget.cpp \
    widget/enhancedgraphicsview.cpp \
    widget/enhancedlistview.cpp \
    widget/enhancedtreeview.cpp \
    widget/hierarchicaltabcontroller.cpp \
    dtt/closeallpoppedwindowstool.cpp \
    dtt/documentmanager.cpp \
    dtt/perspectivewidget.cpp \
    dtt/targetmanager.cpp \
    dtt/tool.cpp \
    dtt/toolactionwrapper.cpp \
    dtt/toolbutton.cpp \
    dtt/perspectivestackedwidget.cpp \
    dtt/mainwindow.cpp

HEADERS +=\
    libh6ncsu_global.h \
    libh6ncsu_global.h \
    widget/dynamicstackedwidget.h \
    widget/enhancedgraphicsview.h \
    widget/enhancedlistview.h \
    widget/enhancedtreeview.h \
    widget/hierarchicaltabcontroller.h \
    dtt/closeallpoppedwindowstool.h \
    dtt/documentmanager.h \
    dtt/perspectivewidget.h \
    dtt/targetmanager.h \
    dtt/tool.h \
    dtt/toolactionwrapper.h \
    dtt/toolbutton.h \
    dtt/perspectivestackedwidget.h \
    dtt/mainwindow.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
