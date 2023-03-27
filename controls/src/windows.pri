win32{
    #The following code works on Windows (at least with all the recent MSVC compilers - didn't test MinGW), Mac OS X (clang) and Linux (GCC).
    #Feel free to omit the first clause and refer to QT_ARCH directly if you don't need Qt 4 support.
    greaterThan(QT_MAJOR_VERSION, 4) {
        TARGET_ARCH=$${QT_ARCH}
    } else {
        TARGET_ARCH=$${QMAKE_HOST.arch}
    }

    contains(TARGET_ARCH, x86_64){
        message("64 bit")
        PLATE_NAME = win64
    }else{
        message("32 bit")
        PLATE_NAME = win32
    }

    CONFIG(release, debug|release){
        message("release")
        DIR_NAME = release
    }else{
        message("debug")
        DIR_NAME = debug
    }

    BASE    = $$PWD/..
    BIN     = $${BASE}/bin
    SRC     = $${BASE}/src
    TEMP    = $${BASE}/temp
    TRD     = $${SRC}/3rdparty/trd

    cache(DIR_NAME, set, DIR_NAME)
    cache(PLATE_NAME, set, PLATE_NAME)
    cache(BASE, set, BASE)
    cache(BIN, set, BIN)
    cache(SRC, set, SRC)
    cache(TEMP, set, TEMP)
    cache(TRD, set, TRD)

    export (DIR_NAME)
    export (PLATE_NAME)
    export (BASE)
    export (SRC)
    export (TRD)

     # 添加管理员权限
    QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"
}
