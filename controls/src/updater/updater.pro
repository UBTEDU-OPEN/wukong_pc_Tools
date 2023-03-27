#-------------------------------------------------
#
# Project created by QtCreator 2019-10-17T17:02:04
#
#-------------------------------------------------
TEMPLATE   = lib
QT        += network
DEFINES   += UPDATER_LIBRARY \
             QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$PWD/QSimpleUpdater/include \
               $${TRD}/quazip/include \
               $${TRD}/zlib/include

win32{
    CONFIG(release, debug|release){
        TARGET  = updater
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS    += -L$${TRD}/zlib/lib/$${PLATE_NAME}/$${DIR_NAME} \
                   -lzlibwapi \
                   -L$${TRD}/quazip/lib/$${PLATE_NAME}/$${DIR_NAME} \
                   -lquazip
    }else{
        TARGET  = updaterd
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS    += -L$${TRD}/zlib/lib/$${PLATE_NAME}/$${DIR_NAME} \
                   -lzlibwapid \
                   -L$${TRD}/quazip/lib/$${PLATE_NAME}/$${DIR_NAME} \
                   -lquazipd
    }
}

include ($$PWD/QSimpleUpdater/QSimpleUpdater.pri)

SOURCES += $$PWD/updaterwindow.cpp \
           $$PWD/urestsignfun.cpp

HEADERS += $$PWD/updater_global.h \
           $$PWD/updaterwindow.h \
           $$PWD/urestsignfun.h \
           $$PWD/uhttpdefine.h

FORMS   += $$PWD/updaterwindow.ui

RESOURCES += \
           $$PWD/styles.qrc \
           $$PWD/fonts.qrc

MOC_DIR         = $${TEMP}/$$TARGET/moc
UI_DIR          = $${TEMP}/$$TARGET/ui
UI_HEADERS_DIR  = $${TEMP}/$$TARGET/ui
UI_SOURCES_DIR  = $${TEMP}/$$TARGET/ui
OBJECTS_DIR     = $${TEMP}/$$TARGET/obj
RCC_DIR         = $${TEMP}/$$TARGET/rcc

win32: LIBS += -L$$PWD/../3rdparty/trd/ubtlib/lib/ -lUbtlib

INCLUDEPATH += $$PWD/../3rdparty/trd/ubtlib/include
DEPENDPATH += $$PWD/../3rdparty/trd/ubtlib/include
