# Copyright 2014-2015 Hallowyn and others.

QT += widgets
CONFIG += c++11

TARGET = h6ncsu
TEMPLATE = lib

DEFINES += LIBH6NCSU_LIBRARY

exists(/usr/bin/ccache):QMAKE_CXX = ccache g++
exists(/usr/bin/ccache):QMAKE_CXXFLAGS += -fdiagnostics-color=always
QMAKE_CXXFLAGS += -Wextra
unix:debug:QMAKE_CXXFLAGS += -ggdb
unix {
  OBJECTS_DIR = ../build-libh6ncsu-unix/obj
  RCC_DIR = ../build-libh6ncsu-unix/rcc
  MOC_DIR = ../build-libh6ncsu-unix/moc
}

INCLUDEPATH += ../qron/libqtpf ../qron/libqtssu
win32:debug:LIBS += -L../build-libqtpf-windows/debug \
  -L../build-libqtpfsql-windows/debug \
  -L../build-libqtssu-windows/debug
win32:release:LIBS += -L../build-libqtpf-windows/release \
  -L../build-libqtpfsql-windows/release \
  -L../build-libqtssu-windows/release
unix:LIBS += -L../qron/libqtpf -L../qron/libqtssu
LIBS += -lqtpf -lqtssu

SOURCES += \
    widget/dynamicstackedwidget.cpp \
    modelview/enhancedgraphicsview.cpp \
    modelview/enhancedlistview.cpp \
    modelview/enhancedtreeview.cpp \
    widget/hierarchicaltabcontroller.cpp \
    dtp/perspectivewidget.cpp \
    dtp/targetmanager.cpp \
    dtp/toolbutton.cpp \
    dtp/perspectivestackedwidget.cpp \
    modelview/shareduiitemwidgetmapper.cpp \
    widget/collapsiblegroupbox.cpp \
    util/failsafeundocommand.cpp \
    widget/enhancedtextedit.cpp \
    widget/enhancedplaintextedit.cpp \
    graphics/graphvizedgegraphicsitem.cpp \
    graphics/graphvizgraphicslayout.cpp \
    widget/interruptiblewaitwidget.cpp \
    dtp/dtpaction.cpp \
    dtp/dtpdocumentmanager.cpp \
    dtp/dtpmainwindow.cpp \
    dtp/closeallpoppedwindowsaction.cpp \
    modelview/dtpgraphicsscene.cpp \
    modelview/dtpgraphicsview.cpp \
    modelview/dtptreeview.cpp \
    dtp/deleteitemaction.cpp \
    dtp/dtpdocumentmanagerwrapper.cpp \
    dtp/createitemaction.cpp \
    dtp/popcloneaction.cpp \
    modelview/shareduiitemproxymodel.cpp

HEADERS +=\
    libh6ncsu_global.h \
    libh6ncsu_global.h \
    widget/dynamicstackedwidget.h \
    modelview/enhancedgraphicsview.h \
    modelview/enhancedlistview.h \
    modelview/enhancedtreeview.h \
    widget/hierarchicaltabcontroller.h \
    dtp/perspectivewidget.h \
    dtp/targetmanager.h \
    dtp/toolbutton.h \
    dtp/perspectivestackedwidget.h \
    modelview/shareduiitemwidgetmapper.h \
    widget/collapsiblegroupbox.h \
    util/failsafeundocommand.h \
    widget/enhancedtextedit.h \
    widget/enhancedplaintextedit.h \
    graphics/graphvizedgegraphicsitem.h \
    graphics/graphvizgraphicslayout.h \
    widget/interruptiblewaitwidget.h \
    dtp/dtpaction.h \
    dtp/closeallpoppedwindowsaction.h \
    dtp/dtpdocumentmanager.h \
    dtp/dtpmainwindow.h \
    modelview/dtptreeview.h \
    modelview/dtpgraphicsscene.h \
    modelview/dtpgraphicsview.h \
    dtp/deleteitemaction.h \
    dtp/dtpdocumentmanagerwrapper.h \
    dtp/createitemaction.h \
    dtp/popcloneaction.h \
    modelview/shareduiitemproxymodel.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    widget/interruptiblewaitwidget.ui
