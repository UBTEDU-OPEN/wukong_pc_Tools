INCLUDEPATH += $$PWD/../trd/qtav/include

macx:{

}else:win32:{
    CONFIG(debug, debug|release){
        LIBS += -L$$PWD/../trd/qtav/lib/win32 -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lcommond -lOpenAL32 \
                        -lswresample -lswscale -lQmlAVd -lQtAVd1 -lQtAVWidgetsd1
    }else{
    }
}

