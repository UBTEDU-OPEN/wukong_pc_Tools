include(windows.pri)
TEMPLATE = subdirs
SUBDIRS += libcontrol \
           sharelog \
           hidapi \
           usbcomm \
           updater \
           controlrobots \

usbcomm.depends = hidapi sharelog
controlrobots.depends = libcontrol sharelog usbcomm updater

TRANSLATIONS += $$PWD/controlrobots/res/ts/jk_zh_CN.ts \
                $$PWD/controlrobots/res/ts/jk_en_US.ts




