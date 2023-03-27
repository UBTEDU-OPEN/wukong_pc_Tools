#-------------------------------------------------
#
# Project created by QtCreator 2015-11-25T17:47:04
#
#-------------------------------------------------
#include ($$(PWD)../plate.pri)

TEMPLATE = lib
QT      += widgets opengl xml
CONFIG  += precompile_header
PRECOMPILED_HEADER += $$PWD/stdafx.h
DEFINES += UBXROBOT3DVIEW_LIBRARY USEXML
DEFINES += FBXSDK_SHARED

INCLUDEPATH += $$(PWD)../common \
               $$(PWD)../UBXEnv \
               $$(PWD)../UBXRobotController \
               $$(PWD)../UBXRobotController/Robot \
               $$(PWD)../LibFBX \
               $${TRD}/fbxsdk/include \
               $${SRC}/ubxbase/common \
               $${SRC}/ubxbase/utils \
               $${SRC}/ubxbase/qutils

win32{
    CONFIG(release, debug|release){
        TARGET  = UBXRobot3DView
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${DESTDIR} \
                -lutils \
                -lqutils \
                -lUBXEnv \
                -lUBXRobotController \
                -lLibFBX \
                -L$${TRD}/fbxsdk/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lglut32 \
                -lglew32 \
                -llibfbxsdk
        PRE_TARGETDEPS += $${DESTDIR}/LibFBX.lib
    }
    CONFIG(debug, debug|release){
        TARGET   = UBXRobot3DViewd
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME} 
        LIBS += -L$${DESTDIR} \
                -lutilsd \
                -lqutilsd \
                -lUBXEnvd \
                -lUBXRobotControllerd \
                -lLibFBXd \
                -L$${TRD}/fbxsdk/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lglut32 \
                -lglew32 \
                -llibfbxsdk
        PRE_TARGETDEPS += $${DESTDIR}/LibFBXd.lib
    }
}

SOURCES += stdafx.cpp \
           IUBXRobot3DView.cpp \
           ubxrobot3dwidget.cpp \
           ubxrobot3ddisplay.cpp \
           ubxrobot3dcontainer.cpp \
           ubxrobot3dcontrolwidget.cpp

HEADERS += stdafx.h \
           ubxrobot3dview_global.h \
           IUBXRobot3DView.h \
           ubxrobot3ddisplay.h \
           ubxrobot3dwidget.h \
           ubxrobot3dcontainer.h \
           ubxrobot3dcontrolwidget.h \
           ubxrobotcontrolinterface.h \
           $$(PWD)../UBXRobotController/UBXRobotController.h

FORMS += ubxrobot3dcontrol.ui \
         ubxrobot3dcontrolwidget.ui

RESOURCES += res.qrc

TRANSLATIONS += $${SRC}/res/languages/UBXRobot3DView_zh_CN.ts \
                $${SRC}/res/languages/UBXRobot3DView_en.ts

#MOC_DIR         = $${TEMP}/$$TARGET/moc
#UI_DIR          = $${TEMP}/$$TARGET/ui
#UI_HEADERS_DIR  = $${TEMP}/$$TARGET/ui
#UI_SOURCES_DIR  = $${TEMP}/$$TARGET/ui
#OBJECTS_DIR     = $${TEMP}/$$TARGET/obj
#RCC_DIR         = $${TEMP}/$$TARGET/rcc
