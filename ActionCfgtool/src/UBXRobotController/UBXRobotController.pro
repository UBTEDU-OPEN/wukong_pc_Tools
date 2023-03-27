#-------------------------------------------------
#
# Project created by QtCreator 2015-12-24T11:02:10
#
#-------------------------------------------------
#include ($$(PWD)../plate.pri)

TEMPLATE  = lib
QT       += core gui xml
CONFIG += precompile_header
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
DEFINES += UBXROBOTCONTROLLER_EXPORTS

INCLUDEPATH += $${SRC}/UBXRobotController/Robot \
               $${SRC}/ubxbase/qutils \
               $$(PWD)../Common \
               $${SRC}/ubxbase/common\
               $${TRD}/libusb/include
PRECOMPILED_HEADER += $$PWD/stdafx.h

win32{
    CONFIG(release, debug|release){
        TARGET  = UBXRobotController
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${DESTDIR} \
                -lutils \
                -L$${TRD}/msvcp/$${PLATE_NAME}/$${DIR_NAME} \
                -luser32 \
                -lsetupapi \
                -L$${TRD}/libusb/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -llibusb-1.0
    }
    CONFIG(debug, debug|release){
        TARGET   = UBXRobotControllerd
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${DESTDIR} \
                -lutilsd \
                -L$${TRD}/msvcp/$${PLATE_NAME}/$${DIR_NAME} \
                -luser32 \
                -lsetupapi \
                -L$${TRD}/libusb/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -llibusb-1.0
    }
}

SOURCES += Robot/RobotController.cpp \
           stdafx.cpp \
           UBXRobotController.cpp \
           Robot/hid.cpp

HEADERS += Robot/RobotController.h \
           stdafx.h \
           UBXRobotController.h \
           Robot/hidapi.h \
           ubxrobotcontroller_global.h

#MOC_DIR         = $${TEMP}/$$TARGET/moc
#UI_DIR          = $${TEMP}/$$TARGET/ui
#UI_HEADERS_DIR  = $${TEMP}/$$TARGET/ui
#UI_SOURCES_DIR  = $${TEMP}/$$TARGET/ui
#OBJECTS_DIR     = $${TEMP}/$$TARGET/obj
#RCC_DIR         = $${TEMP}/$$TARGET/rcc
