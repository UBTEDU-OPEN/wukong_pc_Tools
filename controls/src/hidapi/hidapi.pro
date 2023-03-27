TEMPLATE  = lib
QT       -= gui
CONFIG   += c++11

DEFINES += QHIDAPI_LIBRARY

win32{
    CONFIG(release, debug|release){
        TARGET  = hidapi
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
    }
    CONFIG(debug, debug|release){
        TARGET   = hidapid
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
    }

    LIBS += -L$${TRD}/msvcp/$${PLATE_NAME}/$${DIR_NAME} \
            -lsetupapi
}

SOURCES += \
    qhidapi.cpp \
    qhidapi_p.cpp \
    windows/hid.c

HEADERS += \
    qhidapi_global.h \
    qhidapi.h \
    qhidapi_p.h \
    qhiddeviceinfo.h \
    hidapi.h

MOC_DIR         = $${TEMP}/$$TARGET/moc
UI_DIR          = $${TEMP}/$$TARGET/ui
UI_HEADERS_DIR  = $${TEMP}/$$TARGET/ui
UI_SOURCES_DIR  = $${TEMP}/$$TARGET/ui
OBJECTS_DIR     = $${TEMP}/$$TARGET/obj
RCC_DIR         = $${TEMP}/$$TARGET/rcc
