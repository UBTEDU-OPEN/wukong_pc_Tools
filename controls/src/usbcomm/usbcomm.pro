#-------------------------------------------------
#
# Project created by QtCreator 2019-09-20T15:17:20
#
#-------------------------------------------------
TEMPLATE  = lib
QT       -= gui
DEFINES += QT_DEPRECATED_WARNINGS USBCOMM_LIBRARY

INCLUDEPATH += $${SRC}/sharelog \
               $${SRC}/hidapi \
               $${TRD}/zlib/include \
               $${TRD}/quazip/include

win32{
    CONFIG(release, debug|release){
        TARGET  = usbcomm
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${TRD}/zlib/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lzlibwapi \
                -L$${TRD}/quazip/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lquazip
        LIBS += -L$${DESTDIR} \
                -lsharelog \
                -lhidapi
    }
    CONFIG(debug, debug|release){
        TARGET   = usbcommd
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${TRD}/zlib/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lzlibwapid \
                -L$${TRD}/quazip/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lquazipd
        LIBS += -L$${DESTDIR} \
                -lsharelogd \
                -lhidapid
    }
}

SOURCES +=  usbcomm.cpp \
            cfileinfo.cpp \
    qusbcontrol.cpp

HEADERS +=  usbcomm.h\
            usbcomm_global.h \
            protocoldefine.h \
            cfileinfo.h \
    qusbcontrol.h

MOC_DIR         = $${TEMP}/$$TARGET/moc
UI_DIR          = $${TEMP}/$$TARGET/ui
UI_HEADERS_DIR  = $${TEMP}/$$TARGET/ui
UI_SOURCES_DIR  = $${TEMP}/$$TARGET/ui
OBJECTS_DIR     = $${TEMP}/$$TARGET/obj
RCC_DIR         = $${TEMP}/$$TARGET/rcc
