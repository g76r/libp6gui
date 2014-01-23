# Copyright 2014 Hallowyn and others.

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

HEADERS +=\
    libh6ncsu_global.h \
    libh6ncsu_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
