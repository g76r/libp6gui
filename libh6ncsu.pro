# Copyright 2014 Hallowyn and others.

QT += widgets

TARGET = h6ncsu
TEMPLATE = lib

DEFINES += LIBH6NCSU_LIBRARY

QMAKE_CXXFLAGS += -Wextra
unix:debug:QMAKE_CXXFLAGS += -ggdb
unix {
  OBJECTS_DIR = ../build-libh6ncsu-unix/obj
  RCC_DIR = ../build-libh6ncsu-unix/rcc
  MOC_DIR = ../build-libh6ncsu-unix/moc
}

INCLUDEPATH += ../libqtpf ../libqtssu
win32:debug:LIBS += -L../libqtpf/build-libqtpf-windows/debug \
  -L../libqtpf/build-libqtpfsql-windows/debug \
  -L../build-libqtssu-windows/debug
win32:release:LIBS += -L../libqtpf/build-libqtpf-windows/release \
  -L../libqtpf/build-libqtpfsql-windows/release \
  -L../build-libqtssu-windows/release
unix:LIBS += -L../libqtpf/libqtpf -L../libqtpf/libqtpfsql -L../libqtssu
LIBS += -lqtpf -lqtssu

SOURCES += \
    widget/dynamicstackedwidget.cpp \
    widget/enhancedgraphicsview.cpp \
    modelview/enhancedlistview.cpp \
    modelview/enhancedtreeview.cpp \
    widget/hierarchicaltabcontroller.cpp \
    dtt/closeallpoppedwindowstool.cpp \
    dtt/documentmanager.cpp \
    dtt/perspectivewidget.cpp \
    dtt/targetmanager.cpp \
    dtt/tool.cpp \
    dtt/toolactionwrapper.cpp \
    dtt/toolbutton.cpp \
    dtt/perspectivestackedwidget.cpp \
    dtt/mainwindow.cpp \
    modelview/uiitemwidgetmapper.cpp \
    widget/collapsiblegroupbox.cpp \
    modelview/dtttreeview.cpp

HEADERS +=\
    libh6ncsu_global.h \
    libh6ncsu_global.h \
    widget/dynamicstackedwidget.h \
    widget/enhancedgraphicsview.h \
    modelview/enhancedlistview.h \
    modelview/enhancedtreeview.h \
    widget/hierarchicaltabcontroller.h \
    dtt/closeallpoppedwindowstool.h \
    dtt/documentmanager.h \
    dtt/perspectivewidget.h \
    dtt/targetmanager.h \
    dtt/tool.h \
    dtt/toolactionwrapper.h \
    dtt/toolbutton.h \
    dtt/perspectivestackedwidget.h \
    dtt/mainwindow.h \
    modelview/uiitemwidgetmapper.h \
    widget/collapsiblegroupbox.h \
    modelview/dtttreeview.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
