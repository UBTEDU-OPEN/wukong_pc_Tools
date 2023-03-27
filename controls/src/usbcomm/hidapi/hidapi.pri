INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

!build_hidapi_lib:DEFINES += HIDAPI_NO_LIB

SOURCES += \
    qhidapi.cpp \
    qhidapi_p.cpp

HEADERS += \
    qhidapi.h \
    qhiddeviceinfo.h \
    qhidapi_p.h \
    hidapi.h

unix|win32|macx:contains(DEFINES, USE_LIBUSB) | android {
#    SOURCES += libusb/hid.c
#    INCLUDEPATH += /usr/include/libusb-1.0
#    LIBS += -lusb-1.0
} else {
#    unix:SOURCES += linux/hid.c
    win32:SOURCES += windows/hid.c
    macx:SOURCES += mac/hid.c
}
