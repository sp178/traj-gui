#-------------------------------------------------
#
# Project created by QtCreator 2017-11-27T21:22:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = traj_gui
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    trajshow.cpp

HEADERS += \
    trajshow.h \
    readproject.h
DEFINES +=NODE_EDITOR_SHARED
CONFIG +=c++11
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/release/ -lnodes
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/debug/ -lnodes
else:unix: LIBS += -L/home/sp178/project/build-nodeeditor-Desktop_Qt_5_9_3_GCC_64bit-Debug/lib -lnodes

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/release/ -lboost_system
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/debug/ -lboost_system
else:unix: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lboost_system
LIBS += -L/home/sp178/project/trajproject/plantform -ltraj

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include

INCLUDEPATH += /home/sp178/project/trajproject/include
DEPENDPATH += /home/sp178/project/trajproject/include
