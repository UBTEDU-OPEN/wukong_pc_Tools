#-------------------------------------------------
#
# Project created by QtCreator 2019-07-12T14:06:41
#
#-------------------------------------------------
TEMPLATE  = lib
QT       -= gui
CONFIG   += c++11

DEFINES += SHARELOG_EXPORTS \
           ELPP_AS_DLL \
           ELPP_EXPORT_SYMBOLS \
           ELPP_NO_DEFAULT_LOG_FILE

win32{
    CONFIG(release, debug|release){
        TARGET  = sharelog
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
    }
    CONFIG(debug, debug|release){
        TARGET   = sharelogd
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
    }
}

SOURCES += sharelog.cpp \
           easylogging++.cc

HEADERS += sharelog.h \
           easylogging++.h

MOC_DIR         = $${TEMP}/$$TARGET/moc
UI_DIR          = $${TEMP}/$$TARGET/ui
UI_HEADERS_DIR  = $${TEMP}/$$TARGET/ui
UI_SOURCES_DIR  = $${TEMP}/$$TARGET/ui
OBJECTS_DIR     = $${TEMP}/$$TARGET/obj
RCC_DIR         = $${TEMP}/$$TARGET/rcc
