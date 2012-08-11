QT       += opengl core gui

#CONFIG   += qt release warn_off
CONFIG   += qt debug warn_on
QMAKE_CXXFLAGS_DEBUG +=-Wall
#QMAKE_CXXFLAGS_RELEASE -= -O2
#QMAKE_CXXFLAGS_RELEASE += -O3 -march=native -mtune=native -fomit-frame-pointer

TARGET = qbox2d
TEMPLATE = app

SOURCES += main.cpp\
           mainwindow.cpp \
           items.cpp \
           view.cpp \
           world.cpp \
           worlds/testworld.cpp \
           worlds/exampleworld.cpp \
           worlds/arcanoidworld.cpp \
           qscene.cpp \
           glscene.cpp

HEADERS += mainwindow.h \
           items.h \
           def.h \
           view.h \
           world.h \
           worlds/worlds.h \
           worlds/testworld.h \
           worlds/exampleworld.h \
           worlds/arcanoidworld.h \
           qscene.h \
           glscene.h

FORMS   += mainwindow.ui

MOC_DIR = tmp
OBJECTS_DIR = tmp

INCLUDEPATH += Box2D worlds
QMAKE_LIBDIR += $$PWD/Box2D/lib
LIBS += -lBox2D
