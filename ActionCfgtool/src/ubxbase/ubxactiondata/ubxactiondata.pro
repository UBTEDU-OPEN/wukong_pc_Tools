#-------------------------------------------------
#
# Project created by QtCreator 2017-11-27T11:06:16
#
#-------------------------------------------------
#include ($$(PWD)../../plate.pri)

TEMPLATE = lib
QT      -= core gui
CONFIG  += c++11
DEFINES += UBX_ACTIONDATA_LIB

INCLUDEPATH += $$(PWD)../utils

win32{
    CONFIG(release, debug|release){
        TARGET  = ubxactiondata
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${DESTDIR} \
                -lutils
    }
    CONFIG(debug, debug|release){
        TARGET   = ubxactiondatad
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${DESTDIR} \
                -lutilsd
    }
}

SOURCES += baseblock.cpp \
           flowchartblock.cpp \
           portconnectmap.cpp \
           portinfo.cpp \
           portinfomanager.cpp \
           ubxdatabase.cpp

HEADERS += baseblock.h \
           flowchartblock.h \
           globaldefs.h \
           portconnectmap.h \
           portinfo.h \
           portinfomanager.h \
           processhandle.h \
           rawdata.h \
           ubxdatabase.h

MOC_DIR         = $${TEMP}/$$TARGET/moc
UI_DIR          = $${TEMP}/$$TARGET/ui
UI_HEADERS_DIR  = $${TEMP}/$$TARGET/ui
UI_SOURCES_DIR  = $${TEMP}/$$TARGET/ui
OBJECTS_DIR     = $${TEMP}/$$TARGET/obj
RCC_DIR         = $${TEMP}/$$TARGET/rcc
