
INCLUDEPATH += $${TRD}/quazip/include \
               $${TRD}/zlib/include
DEFINES += UPDATELIB_LIBRARY
win32{
    CONFIG(release, debug|release){
        LIBS    += -L$${TRD}/zlib/lib/$${PLATE_NAME}/$${DIR_NAME} \
                   -lzlibwapi \
                   -L$${TRD}/quazip/lib/$${PLATE_NAME}/$${DIR_NAME} \
                   -lquazip
    }
    CONFIG(debug, debug|release){
        LIBS    += -L$${TRD}/zlib/lib/$${PLATE_NAME}/$${DIR_NAME} \
                   -lzlibwapid \
                   -L$${TRD}/quazip/lib/$${PLATE_NAME}/$${DIR_NAME} \
                   -lquazipd
    }
}

SOURCES     +=  ../updatelib/downloadmanager.cpp \
                ../updatelib/updateManager.cpp \
                ../updatelib/updateparam.cpp

HEADERS     +=  ../updatelib/urldef.h \
                ../updatelib/downloadmanager.h \
                ../updatelib/updateManager.h \
                ../updatelib/updateparam.h \
                ../updatelib/updatelib_global.h
