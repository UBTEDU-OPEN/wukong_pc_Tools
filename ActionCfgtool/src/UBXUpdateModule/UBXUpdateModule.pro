#-------------------------------------------------
#
# Project created by QtCreator 2016-03-09T09:48:16
#
#-------------------------------------------------

QT       += widgets network xml

TARGET = UBXUpdateModule
TEMPLATE = lib

DEFINES += UBXUPDATEMODULE_LIBRARY
DEFINES += USEVERSION

CONFIG += precompile_header
PRECOMPILED_HEADER += $$PWD/stdafx.h

INCLUDEPATH += $$PWD/../zxLib \
    $$PWD/../Common

DEFINES += _AFXDLL

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../zxLib/release/ -lzxLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../zxLib/debug/ -lzxLib
else:unix: LIBS += -L$$OUT_PWD/../zxLib/ -lzxLib

INCLUDEPATH += $$PWD/../zxLib
DEPENDPATH += $$PWD/../zxLib
win32:{
#    LIBS += -L$$PWD/../libs -lAdvAPI32
    LIBS += -L$$PWD/../libs -lsetupapi
    LIBS += -L$$PWD/../libs -luser32
}

macx:{
    LIBS += -L/usr/lib -liconv
}else:win32:{
    LIBS += -L$$PWD/../trd/zlib/lib -lzdll
    LIBS += -L$$PWD/../libs -lversion
}

macx:{
LIBS += -L$$OUT_PWD/../UBXRobotController -lUBXRobotController
}else:win32:{
CONFIG(debug, debug|release){
    LIBS += -L$$OUT_PWD/../UBXRobotController/Debug -lUBXRobotController
}else{
    LIBS += -L$$OUT_PWD/../UBXRobotController/Release -lUBXRobotController
}
}

macx:{
LIBS += -framework IOKit -framework CoreFoundation
}

INCLUDEPATH += $$PWD/../UBXRobotController \
                                  $$PWD/../UBXRobotController/Robot
DEPENDPATH += $$PWD/../UBXRobotController

SOURCES += stdafx.cpp \
            global.cpp \
            updateprogressdlg.cpp \
            $$PWD/../Common/QtHttpOperSync.cpp \
            $$PWD/../Common/Json/json_reader.cpp \
            $$PWD/../Common/Json/json_value.cpp \
            $$PWD/../Common/Json/json_writer.cpp \
            $$PWD/../Common/Util.cpp \
    ubxupdateadministrator.cpp \
    systemembeddedupdatewidget.cpp \
    ../Common/filedirhandle.cpp \
    ../Common/MD5.cpp \
    systempcsoftversionui.cpp \
    videotutorialhandle.cpp \
    cthreadcopyfile.cpp \
    cthreadwaitforudiskshowup.cpp

HEADERS += stdafx.h \
            ubxupdatemodule_global.h \
            global.h \
            $$PWD/../UBXRobotController/UBXRobotController.h \
            updateprogressdlg.h \
            $$PWD/../Common/QtHttpOperSync.h \
            $$PWD/../Common/Json/autolink.h \
            $$PWD/../Common/Json/config.h \
            $$PWD/../Common/Json/features.h \
            $$PWD/../Common/Json/forwards.h \
            $$PWD/../Common/Json/json.h \
            $$PWD/../Common/Json/json_batchallocator.h \
            $$PWD/../Common/Json/reader.h \
            $$PWD/../Common/Json/value.h \
            $$PWD/../Common/Json/writer.h \
            $$PWD/../Common/Util.h \
            $$PWD/../zxLib/zxLib.h \
    ubxupdateadministrator.h \
    ubxupdatemoduleconst.h \
    systemembeddedupdatewidget.h \
            $$PWD/../zxLib/zxLib.h \
    ../Common/filedirhandle.h \
    ../Common/MD5.h \
    systempcsoftversionui.h \
    videotutorialhandle.h \
    cthreadcopyfile.h \
    cthreadwaitforudiskshowup.h

macx:{
SOURCES +=
HEADERS +=
}
#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}

FORMS += \
    updateprogressdlg.ui \
    systemembeddedupdatewidget.ui \

TRANSLATIONS += ../languages/UBXUpdateModule_zh_CN.ts \
        ../languages/UBXUpdateModule_en.ts \

RESOURCES += \
    ubxupdatemodule.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UBXBase/release/ -lUBXBase
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UBXBase/debug/ -lUBXBase
else:unix: LIBS += -L$$OUT_PWD/../UBXBase/ -lUBXBase

INCLUDEPATH += $$PWD/../UBXBase
DEPENDPATH += $$PWD/../UBXBase
