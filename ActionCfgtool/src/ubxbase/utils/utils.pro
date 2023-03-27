#-------------------------------------------------
#
# Project created by QtCreator 2017-11-24T12:19:52
#
#-------------------------------------------------
#include ($$(PWD)../../plate.pri)

TEMPLATE = lib
QT      -= core gui
CONFIG  += c++11
DEFINES += UTILS_LIB

win32{
    CONFIG(release, debug|release){
        TARGET  = utils
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
    }
    CONFIG(debug, debug|release){
        TARGET   = utilsd
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
    }
}

SOURCES += crc32.cpp \
           formatfunc.cpp \
           log4z.cpp \
           notificationcenter.cpp \
           xmlreader.cpp \
           base32.c \
           base64.c

HEADERS += utils_global.h \
           angletransform.h \
           base32.h \
           base64.h \
           basecipher.h \
           cipherbase.h \
           crc32.h \
           formatfunc.h \
           log4z.h \
           notificationcenter.h \
           singleton.h \
           singleton.hpp \
           utilsdef.h \
           xmlreader.h

MOC_DIR         = $${TEMP}/$$TARGET/moc
UI_DIR          = $${TEMP}/$$TARGET/ui
UI_HEADERS_DIR  = $${TEMP}/$$TARGET/ui
UI_SOURCES_DIR  = $${TEMP}/$$TARGET/ui
OBJECTS_DIR     = $${TEMP}/$$TARGET/obj
RCC_DIR         = $${TEMP}/$$TARGET/rcc
