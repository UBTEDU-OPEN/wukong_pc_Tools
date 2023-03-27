win32{
    # 拷贝第三方动态库至程序运行目录下
    # 注意为什么需要用到openssl和openssl1_1_0h？ ps:openssl用于qt的网络请求，openssl1_1_0h用于GDPR认证时数字签名。
    QMAKE_POST_LINK += \
        xcopy /y /s /e /i  \"$${TRD}\openssl\\$${PLATE_NAME}\\$${DIR_NAME}\*.*\"              \"$${DESTDIR}\"                 && \
        xcopy /y /s /e /i  \"$${TRD}\openssl1_1_0h\lib\\$${PLATE_NAME}\\$${DIR_NAME}\*.*\"    \"$${DESTDIR}\"                 && \
        xcopy /y /s /e /i  \"$${TRD}\msvcp\\$${PLATE_NAME}\\$${DIR_NAME}\*.*\"                \"$${DESTDIR}\"                 && \
        xcopy /y /s /e /i  \"$${TRD}\zlib\lib\\$${PLATE_NAME}\\$${DIR_NAME}\*.*\"             \"$${DESTDIR}\"                 && \
        xcopy /y /s /e /i  \"$${TRD}\quazip\lib\\$${PLATE_NAME}\\$${DIR_NAME}\*.*\"           \"$${DESTDIR}\"                 && \
        xcopy /y /s /e /i  \"$${TRD}\QtAv\lib\\$${PLATE_NAME}\\$${DIR_NAME}\*.*\"             \"$${DESTDIR}\"                 && \
        xcopy /y /s /e /i  \"$${TRD}\QtAv\lib\\$${PLATE_NAME}\\qtav-depens-windows\*.*\"      \"$${DESTDIR}\"

    # 拷贝资源文件
    QMAKE_POST_LINK +=  &&\
        xcopy /y /s /e /i  \"$${SRC}\controlrobots\res\qrc\actionlist\*.*\"                 \"$${DESTDIR}\Config\actionlist\"   && \
        copy  /y           \"$${SRC}\controlrobots\res\qrc\playlist.txt\"                   \"$${DESTDIR}\Config\*.txt\"        && \
        copy  /y           \"$${SRC}\controlrobots\res\qrc\playlist_en.txt\"                \"$${DESTDIR}\Config\*.txt\"        && \
        copy  /y           \"$${SRC}\controlrobots\res\qrc\dynamicplaylist.txt\"            \"$${DESTDIR}\Config\*.txt\"        && \
        copy  /y           \"$${SRC}\controlrobots\res\qrc\config.ini\"                     \"$${DESTDIR}\Config\*.ini\"        && \
        xcopy /y /s /e /i  \"$${SRC}\controlrobots\res\qrc\music\*.*\"                      \"$${DESTDIR}\Config\music\"

    # 拷贝Qt运行库至程序运行目录下
    CONFIG(debug, debug|release){
    QMAKE_POST_LINK += && \
        copy  /y            \"$$(QTDIR)\bin\Qt5Guid.dll\"                                  \"$${DESTDIR}\*.dll\"              && \
        copy  /y            \"$$(QTDIR)\bin\Qt5Networkd.dll\"                              \"$${DESTDIR}\*.dll\"              && \
        copy  /y            \"$$(QTDIR)\bin\Qt5Widgetsd.dll\"                              \"$${DESTDIR}\*.dll\"              && \
        copy  /y            \"$$(QTDIR)\bin\Qt5Cored.dll\"                                 \"$${DESTDIR}\*.dll\"              && \
        copy  /y            \"$$(QTDIR)\bin\Qt5OpenGLd.dll\"                               \"$${DESTDIR}\*.dll\"              && \
        xcopy /y /s /e /i   \"$$(QTDIR)\plugins\platforms\qwindowsd.dll\"                  \"$${DESTDIR}\platforms\*.dll\"
    }else{
    QMAKE_POST_LINK += && \
        copy  /y            \"$$(QTDIR)\bin\Qt5Gui.dll\"                                   \"$${DESTDIR}\*.dll\"              && \
        copy  /y            \"$$(QTDIR)\bin\Qt5Network.dll\"                               \"$${DESTDIR}\*.dll\"              && \
        copy  /y            \"$$(QTDIR)\bin\Qt5Widgets.dll\"                               \"$${DESTDIR}\*.dll\"              && \
        copy  /y            \"$$(QTDIR)\bin\Qt5Core.dll\"                                  \"$${DESTDIR}\*.dll\"              && \
        copy  /y            \"$$(QTDIR)\bin\Qt5OpenGL.dll\"                                \"$${DESTDIR}\*.dll\"              && \
        xcopy /y /s /e /i   \"$$(QTDIR)\plugins\platforms\qwindows.dll\"                   \"$${DESTDIR}\platforms\*.dll\"
    }
}
