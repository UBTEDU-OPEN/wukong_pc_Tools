#-------------------------------------------------
#
# Project created by QtCreator 2016-10-28T18:02:14
#
#-------------------------------------------------
#include ($$(PWD)../plate.pri)

TEMPLATE = lib
QT      += core gui widgets xml
CONFIG += precompile_header
DEFINES += UBXEVENTEDITOR_LIBRARY
PRECOMPILED_HEADER += $$PWD/stdafx.h

INCLUDEPATH += $$(PWD)../common \
               $$(PWD)../LibFlowControl \
               $$(PWD)../UBXEnv \
               $${SRC}/ubxbase/utils \
               $${SRC}/ubxbase/qutils

win32{
    CONFIG(release, debug|release){
        TARGET  = UBXEventEditor
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${DESTDIR} \
                -lutils \
                -lqutils \
                -lUBXEnv
    }
    CONFIG(debug, debug|release){
        TARGET   = UBXEventEditord
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${DESTDIR} \
                -lutilsd \
                -lqutilsd \
                -lUBXEnvd
    }
}

SOURCES += stdafx.cpp \
           ubxeventeditorwidget.cpp

HEADERS += ubxeventeditor.h\
           ubxeventeditor_global.h \
           stdafx.h \
           ubxeventeditorwidget.h \

FORMS += ubxeventeditorwidget.ui

RESOURCES += ubxeventeditor.qrc

TRANSLATIONS += $${SRC}/res/languages/UBXEventEditor_zh_CN.ts \
                $${SRC}/res/languages/UBXEventEditor_en.ts

#MOC_DIR         = $${TEMP}/$$TARGET/moc
#UI_DIR          = $${TEMP}/$$TARGET/ui
#UI_HEADERS_DIR  = $${TEMP}/$$TARGET/ui
#UI_SOURCES_DIR  = $${TEMP}/$$TARGET/ui
#OBJECTS_DIR     = $${TEMP}/$$TARGET/obj
#RCC_DIR         = $${TEMP}/$$TARGET/rcc

