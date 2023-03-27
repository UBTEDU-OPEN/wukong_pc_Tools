# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------
#include ($$(PWD)../plate.pri)

TEMPLATE = lib
QT += core widgets gui
DEFINES += UBXUIBASE_LIB

INCLUDEPATH += $$(PWD)/widget \
               $${SRC}/ubxbase/qutils

win32{
    CONFIG(release, debug|release){
        TARGET  = UBXUiBase
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${DESTDIR} \
                -lqutils
    }
    CONFIG(debug, debug|release){
        TARGET   = UBXUiBased
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${DESTDIR} \
                -lqutilsd
    }
}

HEADERS += ubxuibase_global.h \
           widget/batteryicon.h \
           widget/mailregexp.h \
           widget/mycombobox.h \
           widget/phonenumregexp.h \
           widget/progressdialog.h \
           widget/ubxwidgetconst.h \
           widget/ubxwidget.h \
           widget/ubxmultipage.h \
           widget/ubxdialog.h \
           widget/QRoundProgressBar.h \
           widget/mytitlebar.h \
           widget/messagebox.h \
           widget/menu.h \
           widget/cubxintvalidator.h \
           widget/ctablewidget.h \
           widget/cimagelabel.h \
           widget/checkboxlabel.h \
           widget/ccommentwidget.h
SOURCES += widget/batteryicon.cpp \
           widget/ccommentwidget.cpp \
           widget/checkboxlabel.cpp \
           widget/cimagelabel.cpp \
           widget/ctablewidget.cpp \
           widget/cubxintvalidator.cpp \
           widget/mailregexp.cpp \
           widget/menu.cpp \
           widget/messagebox.cpp \
           widget/mycombobox.cpp \
           widget/mytitlebar.cpp \
           widget/phonenumregexp.cpp \
           widget/progressdialog.cpp \
           widget/QRoundProgressBar.cpp \
           widget/ubxdialog.cpp \
           widget/ubxmultipage.cpp \
           widget/ubxwidget.cpp
FORMS +=   ui/ccommentwidget.ui \
           ui/ubxmultipage.ui \
           ui/mytitlebar.ui

#MOC_DIR         = $${TEMP}/$$TARGET/moc
#UI_DIR          = $${TEMP}/$$TARGET/ui
#UI_HEADERS_DIR  = $${TEMP}/$$TARGET/ui
#UI_SOURCES_DIR  = $${TEMP}/$$TARGET/ui
#OBJECTS_DIR     = $${TEMP}/$$TARGET/obj
#RCC_DIR         = $${TEMP}/$$TARGET/rcc