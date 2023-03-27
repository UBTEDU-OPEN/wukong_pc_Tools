#-------------------------------------------------
#
# Project created by QtCreator 2017-11-27T16:11:28
#
#-------------------------------------------------

QT       -= core gui

TARGET = ubxsocket
TEMPLATE = lib
DESTDIR = ../../bin/osx/
DEFINES += UBXSOCKET_LIB

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
    ../../src/ubxsocket/proxysocket.cpp \
    ../../src/ubxsocket/sockethelper.cpp \
    ../../src/ubxsocket/socketutil.cpp \
    ../../src/ubxsocket/streamingsocket.cpp

HEADERS += \
    ../../include/ubxsocket/globaldefs.h \
    ../../include/ubxsocket/proxyconfig.h \
    ../../include/ubxsocket/proxysocket.h \
    ../../include/ubxsocket/sockethelper.h \
    ../../include/ubxsocket/socketutil.h \
    ../../include/ubxsocket/streamingsocket.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/ -lutils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/ -lutilsd
else:macx: LIBS += -L$$PWD/../../lib/ -lutils

INCLUDEPATH += $$PWD/../../include/utils
DEPENDPATH += $$PWD/../../include/utils

INCLUDEPATH += $$PWD/../../include/ubxsocket
DEPENDPATH += $$PWD/../../include/ubxsocket
