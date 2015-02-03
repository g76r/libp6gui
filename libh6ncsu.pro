# Copyright 2014-2015 Hallowyn and others.

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
win32:debug:LIBS += -L../build-libqtpf-windows/libqtpf/debug \
  -L../build-libqtpf-windows/libqtpfsql/debug \
  -L../build-libqtssu-windows/debug
win32:release:LIBS += -L../build-libqtpf-windows/libqtpf/release \
  -L../build-libqtpf-windows/libqtpfsql/release \
  -L../build-libqtssu-windows/release
unix:LIBS += -L../libqtpf/libqtpf -L../libqtpf/libqtpfsql -L../libqtssu
LIBS += -lqtpf -lqtssu

SOURCES += \
    widget/dynamicstackedwidget.cpp \
    modelview/enhancedgraphicsview.cpp \
    modelview/enhancedlistview.cpp \
    modelview/enhancedtreeview.cpp \
    widget/hierarchicaltabcontroller.cpp \
    dtt/closeallpoppedwindowstool.cpp \
    dtt/perspectivewidget.cpp \
    dtt/targetmanager.cpp \
    dtt/tool.cpp \
    dtt/toolactionwrapper.cpp \
    dtt/toolbutton.cpp \
    dtt/perspectivestackedwidget.cpp \
    dtt/mainwindow.cpp \
    modelview/shareduiitemwidgetmapper.cpp \
    widget/collapsiblegroupbox.cpp \
    modelview/dtttreeview.cpp \
    modelview/shareduigraphicsitem.cpp \
    modelview/dttgraphicsview.cpp \
    modelview/dttgraphicsscene.cpp \
    dtt/shareduiitemseditablemodel.cpp \
    util/failsafeundocommand.cpp \
    widget/enhancedtextedit.cpp \
    widget/enhancedplaintextedit.cpp \
    dtt/dttdocumentmanager.cpp

HEADERS +=\
    libh6ncsu_global.h \
    libh6ncsu_global.h \
    widget/dynamicstackedwidget.h \
    modelview/enhancedgraphicsview.h \
    modelview/enhancedlistview.h \
    modelview/enhancedtreeview.h \
    widget/hierarchicaltabcontroller.h \
    dtt/closeallpoppedwindowstool.h \
    dtt/perspectivewidget.h \
    dtt/targetmanager.h \
    dtt/tool.h \
    dtt/toolactionwrapper.h \
    dtt/toolbutton.h \
    dtt/perspectivestackedwidget.h \
    dtt/mainwindow.h \
    modelview/shareduiitemwidgetmapper.h \
    widget/collapsiblegroupbox.h \
    modelview/dtttreeview.h \
    modelview/shareduigraphicsitem.h \
    modelview/dttgraphicsview.h \
    modelview/dttgraphicsscene.h \
    dtt/shareduiitemseditablemodel.h \
    util/failsafeundocommand.h \
    widget/enhancedtextedit.h \
    widget/enhancedplaintextedit.h \
    dtt/dttdocumentmanager.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
