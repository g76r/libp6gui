# Copyright 2014-2022 Hallowyn, Gregoire Barbier and others.
# This file is part of libpumpkin, see <http://libpumpkin.g76r.eu/>.
# Libpumpkin is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# Libpumpkin is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
# You should have received a copy of the GNU Affero General Public License
# along with libpumpkin.  If not, see <http://www.gnu.org/licenses/>.

QT += widgets
CONFIG += largefile c++17 c++14 c++11 force_debug_info

TARGET = p6gui
TEMPLATE = lib

TARGET_OS=default
unix: TARGET_OS=unix
linux: TARGET_OS=linux
android: TARGET_OS=android
macx: TARGET_OS=macx
win32: TARGET_OS=win32
BUILD_TYPE=unknown
CONFIG(debug,debug|release): BUILD_TYPE=debug
CONFIG(release,debug|release): BUILD_TYPE=release

DEFINES += LIBP6GUI_LIBRARY

exists(/usr/bin/ccache):QMAKE_CXX = ccache $$QMAKE_CXX
exists(/usr/bin/ccache):QMAKE_CXXFLAGS += -fdiagnostics-color=always
QMAKE_CXXFLAGS += -Wextra -Woverloaded-virtual \
  -Wfloat-equal -Wdouble-promotion -Wimplicit-fallthrough=5 -Wtrampolines \
  -Wduplicated-branches -Wduplicated-cond -Wlogical-op \
  -Wno-padded -Wno-deprecated-copy -Wsuggest-attribute=noreturn \
  -ggdb
CONFIG(debug,debug|release):QMAKE_CXXFLAGS += -ggdb

OBJECTS_DIR = ../build-$$TARGET-$$TARGET_OS/$$BUILD_TYPE/obj
UI_DIR = ../build-$$TARGET-$$TARGET_OS/$$BUILD_TYPE/ui
UI_HEADERS_DIR = ../build-$$TARGET-$$TARGET_OS/$$BUILD_TYPE/ui/include
UI_SOURCES_DIR = ../build-$$TARGET-$$TARGET_OS/$$BUILD_TYPE/ui/src
RCC_DIR = ../build-$$TARGET-$$TARGET_OS/$$BUILD_TYPE/rcc
MOC_DIR = ../build-$$TARGET-$$TARGET_OS/$$BUILD_TYPE/moc
DESTDIR = ../build-$$TARGET-$$TARGET_OS/$$BUILD_TYPE
#QMAKE_CXXFLAGS += -O0 -pg -fprofile-arcs -ftest-coverage
#QMAKE_LFLAGS += -pg -fprofile-arcs

# dependency libs
INCLUDEPATH += ../libqtpf ../libp6core
LIBS += -L../build-qtpf-$$TARGET_OS/$$BUILD_TYPE \
  -L../build-p6core-$$TARGET_OS/$$BUILD_TYPE
LIBS += -lqtpf -lp6core

SOURCES *= \
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
    modelview/copyviewashtmlaction.cpp \
    graphics/dtpgraphicsitem.cpp \
    widget/multichoicecombobox.cpp \
    graphics/graphicsitemutils.cpp \
    modelview/decoratedstringlistdiffmodel.cpp \
    util/undotransaction.cpp \
    util/useractivitydetector.cpp

HEADERS *=\
    libp6gui_global.h \
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
    modelview/copyviewashtmlaction.h \
    graphics/dtpgraphicsitem.h \
    widget/multichoicecombobox.h \
    graphics/graphicsitemutils.h \
    modelview/decoratedstringlistdiffmodel.h \
    util/undotransaction.h \
    util/useractivitydetector.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS *= \
    widget/interruptiblewaitwidget.ui


HEADERS *=\
    libp6gui_global.h \
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
    INSTALLS *= target
}

FORMS *= \
    widget/interruptiblewaitwidget.ui
