#-------------------------------------------------
#
# Project created by QtCreator 2019-09-18T14:57:34
#
#-------------------------------------------------
QT       += core gui network
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
# add c++
greaterThan(QT_MAJOR_VERSION, 4) {
    CONFIG += c++11
}else{
    QMAKE_CXXFLAGS += -std=c++11
    QMAKE_CXXFLAGS_DEBUG += -std=c++11
}

# 添加 QtAV 相关头文件
INCLUDEPATH += $${TRD}/QtAv/include \
               $${TRD}/QtAv/include/QtAV \
               $${TRD}/QtAv/include/QtAVWidgets
# 添加 zip 相关头文件
INCLUDEPATH += $${TRD}/quazip/include \
               $${TRD}/zlib/include
INCLUDEPATH += $${SRC}/libcontrol/src \
               $${SRC}/sharelog \
               $${SRC}/usbcomm \
               $${SRC}/hidapi \
               $${SRC}/controlrobots/form \
               $${SRC}/controlrobots/player \
               $${SRC}/updater

win32{
    TARGET  = controlrobots
    CONFIG(release, debug|release){
        TARGET  = controlrobots
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${DESTDIR} \
                -lcontrol \
                -lsharelog \
                -lusbcomm \
                -lupdater
        # add lib qtav
        LIBS += -L$${TRD}/QtAv/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lQtAV1 \
                -lQtAVWidgets1
        # add lib zip
        LIBS += -L$${TRD}/zlib/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lzlibwapi \
                -L$${TRD}/quazip/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lquazip
    }else{
        TARGET = $$join(TARGET,,,d)
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${DESTDIR} \
                -lcontrold \
                -lsharelogd \
                -lusbcommd \
                -lupdaterd
        # add lib qtav
        LIBS += -L$${TRD}/QtAv/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lQtAVd1 \
                -lQtAVWidgetsd1
        # add lib zip
        LIBS += -L$${TRD}/zlib/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lzlibwapid \
                -L$${TRD}/quazip/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lquazipd
    }

    LIBS += -L$${TRD}/msvcp/$${PLATE_NAME}/$${DIR_NAME} \
            -luser32 \
            -lsetupapi
}

include (copylib_win.pri)
include (version.pri)

RESOURCES += \
    res/res.qrc

HEADERS  += $$PWD/ucontroldialog.h \
            $$PWD/mystyle.h \
            $$PWD/singleinstance.h \
            $$PWD/cpubfun.h \
            $$PWD/pubdef.h \
            $$PWD/form/addplaydlg.h \
            $$PWD/form/assignactdev.h \
            $$PWD/form/editsongdlg.h \
            $$PWD/form/udpsendfiledialog.h \
            $$PWD/form/dynamicplaydlg.h \
            $$PWD/form/qmessagedlg.h \
            $$PWD/form/qsettingdlg.h \
            $$PWD/player/playerwindow.h \
            $$PWD/cfgini.h \
            $$PWD/qtablecellwidget.h

SOURCES +=  $$PWD/main.cpp\
            $$PWD/ucontroldialog.cpp \
            $$PWD/singleinstance.cpp \
            $$PWD/cpubfun.cpp \
            $$PWD/form/addplaydlg.cpp \
            $$PWD/form/assignactdev.cpp \
            $$PWD/form/editsongdlg.cpp \
            $$PWD/form/udpsendfiledialog.cpp \
            $$PWD/form/dynamicplaydlg.cpp \
            $$PWD/form/qmessagedlg.cpp \
            $$PWD/form/qsettingdlg.cpp \
            $$PWD/player/playerwindow.cpp \
            $$PWD/cfgini.cpp \
            $$PWD/qtablecellwidget.cpp

FORMS    += $$PWD/ucontroldialog.ui \
            $$PWD/form/addplaydlg.ui \
            $$PWD/form/assignactdev.ui \
            $$PWD/form/editsongdlg.ui \
            $$PWD/form/udpsendfiledialog.ui \
            $$PWD/form/dynamicplaydlg.ui \
            $$PWD/form/qmessagedlg.ui \
            $$PWD/form/qsettingdlg.ui

MOC_DIR         = $${TEMP}/$$TARGET/moc
UI_DIR          = $${TEMP}/$$TARGET/ui
UI_HEADERS_DIR  = $${TEMP}/$$TARGET/ui
UI_SOURCES_DIR  = $${TEMP}/$$TARGET/ui
OBJECTS_DIR     = $${TEMP}/$$TARGET/obj
RCC_DIR         = $${TEMP}/$$TARGET/rcc

win32: LIBS += -L$$PWD/../3rdparty/trd/ubtlib/lib/ -lUbtlib

INCLUDEPATH += $$PWD/../3rdparty/trd/ubtlib/include
DEPENDPATH += $$PWD/../3rdparty/trd/ubtlib/include
