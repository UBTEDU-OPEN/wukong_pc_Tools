#-------------------------------------------------
#
# Project created by QtCreator 2018-09-15T10:23:38
#
#-------------------------------------------------
TEMPLATE = lib
QT       += widgets
DEFINES += LIBCONTROL_LIBRARY QT_DEPRECATED_WARNINGS

include (libcontrol.pri)

# platform attributes
macx{
    TARGET       = control
    DLLDESTDIR = $${BIN}/osx
}
unix:!macx{
    TARGET       = control
    DLLDESTDIR = $${BIN}/linux
}
win32{
    CONFIG(release, debug|release){
        TARGET  = control
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
    }
    CONFIG(debug, debug|release){
        TARGET   = controld
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
    }
}

MOC_DIR         = $${TEMP}/$$TARGET/moc
UI_DIR          = $${TEMP}/$$TARGET/ui
UI_HEADERS_DIR  = $${TEMP}/$$TARGET/ui
UI_SOURCES_DIR  = $${TEMP}/$$TARGET/ui
OBJECTS_DIR     = $${TEMP}/$$TARGET/obj
RCC_DIR         = $${TEMP}/$$TARGET/rcc
