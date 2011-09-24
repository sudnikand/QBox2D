QT       += opengl core gui

CONFIG   += qt release warn_off
#QMAKE_CXXFLAGS_RELEASE -= -O2
#QMAKE_CXXFLAGS_RELEASE += -O3 -march=native -mtune=native -fomit-frame-pointer

TARGET = qbox2d
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp \
           qbox2ditems.cpp

HEADERS += mainwindow.h \
           qbox2ditems.h

FORMS   += mainwindow.ui

MOC_DIR = tmp
OBJECTS_DIR = tmp

INCLUDEPATH += Box2D
QMAKE_LIBDIR += $$PWD/Box2D/lib
LIBS += -lBox2D
