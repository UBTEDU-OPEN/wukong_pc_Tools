#-------------------------------------------------
#
# Project created by QtCreator 2017-11-27T15:59:15
#
#-------------------------------------------------
#include ($$(PWD)../../plate.pri)

TEMPLATE = lib
QT      -= core gui
CONFIG  += c++11
DEFINES += UBXHTTPCLIENT_LIB BUILDING_CURLPP

INCLUDEPATH += $$(PWD)../common \
               $$(PWD)../utils \
               $${TRD}/curlpp/include \
               $${TRD}/curl/include

win32{
    CONFIG(release, debug|release){
        TARGET  = ubxhttpclient
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${DESTDIR} \
                -lutils \
                -L$${TRD}/curl/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -llibcurl \
                -L$${TRD}/curlpp/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lcurlpp
    }
    CONFIG(debug, debug|release){
        TARGET   = ubxhttpclientd
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${DESTDIR} \
                -lutilsd \
                -L$${TRD}/curl/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -llibcurld \
                -L$${TRD}/curlpp/lib/$${PLATE_NAME}/$${DIR_NAME}\
                -lcurlppd
    }
}

SOURCES += httpbase.cpp \
           httpclient.cpp \
           httpdownload.cpp \
           httprequest.cpp \
           httpresponse.cpp

HEADERS += httpbase.h \
           httpclient.h \
           httpdownload.h \
           httprequest.h \
           httpresponse.h \
           ubxhttp \
           ubxhttpclientdef.h

MOC_DIR         = $${TEMP}/$$TARGET/moc
UI_DIR          = $${TEMP}/$$TARGET/ui
UI_HEADERS_DIR  = $${TEMP}/$$TARGET/ui
UI_SOURCES_DIR  = $${TEMP}/$$TARGET/ui
OBJECTS_DIR     = $${TEMP}/$$TARGET/obj
RCC_DIR         = $${TEMP}/$$TARGET/rcc
