# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------
#include ($$(PWD)../../plate.pri)

TEMPLATE = lib
QT += core network
DEFINES += SMTP_LIB

win32{
    CONFIG(release, debug|release){
        TARGET  = smtp
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
    }
    CONFIG(debug, debug|release){
        TARGET   = smtpd
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
    }
}

HEADERS += mimeinlinefile.h \
           mimemessage.h \
           mimetext.h \
           smtpexports.h \
           emailaddress.h \
           mimeattachment.h \
           mimecontentformatter.h \
           mimefile.h \
           mimehtml.h \
           mimemultipart.h \
           mimepart.h \
           quotedprintable.h \
           smtpclient.h \
           smtp_global.h
SOURCES += emailaddress.cpp \
           mimeattachment.cpp \
           mimecontentformatter.cpp \
           mimefile.cpp \
           mimehtml.cpp \
           mimeinlinefile.cpp \
           mimemessage.cpp \
           mimemultipart.cpp \
           mimepart.cpp \
           mimetext.cpp \
           quotedprintable.cpp \
           smtpclient.cpp

MOC_DIR         = $${TEMP}/$$TARGET/moc
UI_DIR          = $${TEMP}/$$TARGET/ui
UI_HEADERS_DIR  = $${TEMP}/$$TARGET/ui
UI_SOURCES_DIR  = $${TEMP}/$$TARGET/ui
OBJECTS_DIR     = $${TEMP}/$$TARGET/obj
RCC_DIR         = $${TEMP}/$$TARGET/rcc