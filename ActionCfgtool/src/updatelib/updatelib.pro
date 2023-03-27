#-------------------------------------------------
#
# Project created by QtCreator 2019-06-24T15:52:50
#
#-------------------------------------------------
QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
DEFINES += QT_DEPRECATED_WARNINGS

win32{
    CONFIG(release, debug|release){
        TARGET  = updatelib
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
    }
    CONFIG(debug, debug|release){
        TARGET   = updatelibd
        DESTDIR  = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
    }
}

include (updateManager.pri)

#MOC_DIR         = $${TEMP}/$$TARGET/moc
#UI_DIR          = $${TEMP}/$$TARGET/ui
#UI_HEADERS_DIR  = $${TEMP}/$$TARGET/ui
#UI_SOURCES_DIR  = $${TEMP}/$$TARGET/ui
#OBJECTS_DIR     = $${TEMP}/$$TARGET/obj
#RCC_DIR         = $${TEMP}/$$TARGET/rcc
win32: LIBS += -L$$PWD/../3rdparty/trd/ubtlib/lib/ -lUbtlib

INCLUDEPATH += $$PWD/../3rdparty/trd/ubtlib/include
DEPENDPATH += $$PWD/../3rdparty/trd/ubtlib/include
