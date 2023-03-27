
QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += QT_DEPRECATED_WARNINGS

TEMPLATE = app
INCLUDEPATH += $${SRC}/updatelib

win32{
    CONFIG(release, debug|release){
        TARGET  = update
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS   += -L$${DESTDIR} \
                  -lupdatelib
    }
    CONFIG(debug, debug|release){
        TARGET   = updated
        DESTDIR  = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS   += -L$${DESTDIR} \
                  -lupdatelibd
    }
}

SOURCES += main.cpp

#MOC_DIR         = $${TEMP}/$$TARGET/moc
#UI_DIR          = $${TEMP}/$$TARGET/ui
#UI_HEADERS_DIR  = $${TEMP}/$$TARGET/ui
#UI_SOURCES_DIR  = $${TEMP}/$$TARGET/ui
#OBJECTS_DIR     = $${TEMP}/$$TARGET/obj
#RCC_DIR         = $${TEMP}/$$TARGET/rcc
