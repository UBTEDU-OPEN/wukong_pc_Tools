#-------------------------------------------------
#
# Project created by QtCreator 2016-08-22T16:59:47
#
#-------------------------------------------------
include ($$(PWD)../plate.pri)

TEMPLATE = lib
QT      += core gui multimedia multimediawidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += precompile_header
DEFINES += UBXPLAYER_LIBRARY

INCLUDEPATH += $$(PWD)../common \
               $$(PWD)../UBXEnv \
               $${SRC}/ubxbase/utils

win32{
    CONFIG(release, debug|release){
        TARGET  = UBXPlayer
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${DESTDIR} \
                -lutils \
                -lUBXEnv
    }
    CONFIG(debug, debug|release){
        TARGET   = UBXPlayerd
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${DESTDIR} \
                -lutilsd \
                -lUBXEnvd
    }
}

SOURCES += ubxvideoplayer.cpp \
           videoplayercontrlwidget.cpp \
           ubxslider.cpp \
           ubxvideooutputitem.cpp \
           videocapture.cpp

HEADERS += ubxplayer_global.h \
           ubxvideoplayer.h \
           ubxplayerconst.h \
           videoplayercontrlwidget.h \
           ubxslider.h \
           ubxvideooutputitem.h \
           videocapture.h

FORMS += ubxvideoplayer.ui \
         videoplayercontrlwidget.ui

RESOURCES += res.qrc

TRANSLATIONS += $${SRC}/res/languages/UBXPlayer_zh_CN.ts \
                $${SRC}/res/languages/UBXPlayer_en.ts

MOC_DIR         = $${TEMP}/$$TARGET/moc
UI_DIR          = $${TEMP}/$$TARGET/ui
UI_HEADERS_DIR  = $${TEMP}/$$TARGET/ui
UI_SOURCES_DIR  = $${TEMP}/$$TARGET/ui
OBJECTS_DIR     = $${TEMP}/$$TARGET/obj
RCC_DIR         = $${TEMP}/$$TARGET/rcc
