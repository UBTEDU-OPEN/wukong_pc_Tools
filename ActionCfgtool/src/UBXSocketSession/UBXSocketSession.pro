#-------------------------------------------------
#
# Project created by QtCreator 2016-11-29T14:30:55
#
#-------------------------------------------------
#include ($$(PWD)../plate.pri)

TEMPLATE  = lib
QT       -= gui
QT       += network xml
CONFIG   += c++11
DEFINES  += UBXSOCKETSESSION_LIBRARY

INCLUDEPATH += $$(PWD)../common \
               $$(PWD)../UBXEnv \
               $$(PWD)../Protocol \
               $$(PWD)../Protocol/src \
               $${TRD}/protolbuf/include \
               $${SRC}/ubxbase/common \
               $${SRC}/ubxbase/utils

win32{
    CONFIG(release, debug|release){
        TARGET  = UBXSocketSession
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${DESTDIR} \
                -lutils \
                -lUBXEnv \
                -lProtocol \
                -L$${TRD}/protolbuf/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -llibprotobuf
    }
    CONFIG(debug, debug|release){
        TARGET   = UBXSocketSessiond
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${DESTDIR} \
                -lutilsd \
                -lUBXEnvd \
                -lProtocold \
                -L$${TRD}/protolbuf/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -llibprotobufd
    }
}

SOURCES += ubxsocketsession.cpp \
           stdafx.cpp \
           UBXSocketSessionImpl.cpp \
           UBXBroadcastSocketImpl.cpp \
           UBXSendFileSocketImpl.cpp \
           UBXUpLoadSocketImpl.cpp \
           Header.cpp

HEADERS +=  ubxsocketsession.h\
            ubxsocketsession_global.h \
            stdafx.h \
            UBXSocketSessionImpl.h \
            UBXBroadcastSocket.h \
            UBXBroadcastSocketImpl.h \
            UBXSendFileSocketImpl.h \
            UBXUpLoadSocketImpl.h \
            IUBXTransFileSession.h \
            IUBXSocketSession.h \
            Header.h

#MOC_DIR         = $${TEMP}/$$TARGET/moc
#UI_DIR          = $${TEMP}/$$TARGET/ui
#UI_HEADERS_DIR  = $${TEMP}/$$TARGET/ui
#UI_SOURCES_DIR  = $${TEMP}/$$TARGET/ui
#OBJECTS_DIR     = $${TEMP}/$$TARGET/obj
#RCC_DIR         = $${TEMP}/$$TARGET/rcc
