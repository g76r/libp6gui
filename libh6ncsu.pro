# Copyright 2014-2016 Hallowyn and others.
# This file is part of libh6ncsu, see <https://gitlab.com/g76r/libh6ncsu>.
# Libh6ncsu is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# Libh6ncsu is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
# You should have received a copy of the GNU Affero General Public License
# along with libh6ncsu.  If not, see <http://www.gnu.org/licenses/>.

QT += widgets
CONFIG += c++11

TARGET = h6ncsu
TEMPLATE = lib

DEFINES += LIBH6NCSU_LIBRARY

exists(/usr/bin/ccache):QMAKE_CXX = ccache g++
exists(/usr/bin/ccache):QMAKE_CXXFLAGS += -fdiagnostics-color=always
QMAKE_CXXFLAGS += -Wextra -Woverloaded-virtual
unix:CONFIG(debug,debug|release):QMAKE_CXXFLAGS += -ggdb
unix {
  UI_DIR = ../build-libh6ncsu-unix/ui
  UI_HEADERS_DIR = ../build-libh6ncsu-unix/ui/include
  UI_SOURCES_DIR = ../build-libh6ncsu-unix/ui/src
  OBJECTS_DIR = ../build-libh6ncsu-unix/obj
  RCC_DIR = ../build-libh6ncsu-unix/rcc
  MOC_DIR = ../build-libh6ncsu-unix/moc
  #QMAKE_CXXFLAGS += -O0 -pg -fprofile-arcs -ftest-coverage
  #QMAKE_LFLAGS += -pg -fprofile-arcs
}

INCLUDEPATH += ../libqtssu
win32:CONFIG(debug,debug|release):LIBS += -L../build-libqtssu-windows/debug
win32:CONFIG(release,debug|release):LIBS += -L../build-libqtssu-windows/release
unix:LIBS += -L../libqtssu
LIBS += -lqtssu

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
    modelview/shareduiitemproxymodel.cpp \
    widget/enhancedtoolbutton.cpp \
    dtp/targetkeepingmenu.cpp \
    widget/widgetutils.cpp \
    modelview/paramsetview.cpp \
    widget/responsiveapplication.cpp \
    modelview/comboboxshareduiitemdelegate.cpp \
    modelview/dtptableview.cpp \
    modelview/enhancedtableview.cpp \
    modelview/dtpmatrixview.cpp \
    modelview/copyviewashtmlaction.cpp

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
    modelview/shareduiitemproxymodel.h \
    widget/enhancedtoolbutton.h \
    dtp/targetkeepingmenu.h \
    widget/widgetutils.h \
    modelview/paramsetview.h \
    widget/responsiveapplication.h \
    modelview/comboboxshareduiitemdelegate.h \
    modelview/dtptableview.h \
    modelview/enhancedtableview.h \
    modelview/dtpmatrixview.h \
    modelview/copyviewashtmlaction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    widget/interruptiblewaitwidget.ui
