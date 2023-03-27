#-------------------------------------------------
#
# Project created by QtCreator 2015-12-03T13:43:23
#
#-------------------------------------------------
#include ($$(PWD)../plate.pri)

TEMPLATE = lib
QT      += core gui widgets multimedia xml
CONFIG  += precompile_header
PRECOMPILED_HEADER = stable.h
DEFINES += MOTIONEDITOR_LIB USEXML

INCLUDEPATH += $$(PWD)../common \
               $$(PWD)../UBXEnv \
               $$(PWD)../mp3proj \
               $$(PWD)../UBXSocketSession \
               $$(PWD)../UBXRobot3DView \
               $${TRD}/libmpg123/include \
               $${TRD}/libsndfile/include \
               $${SRC}/ubxbase/common \
               $${SRC}/ubxbase/utils \
               $${SRC}/ubxbase/ubxactiondata \
               $${SRC}/ubxbase/qutils

win32{
    CONFIG(release, debug|release){
        TARGET  = UBXMotionEditor
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${DESTDIR} \
                -lutils \
                -lqutils \
                -lUBXEnv \
                -lubxactiondata \
                -lUBXRobot3DView \
                -lmp3proj \
                -lUBXSocketSession \
                -L$${TRD}/libmpg123/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -llibmpg123 \
                -L$${TRD}/libsndfile/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -llibsndfile-1
    }
    CONFIG(debug, debug|release){
        TARGET   = UBXMotionEditord
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${DESTDIR} \
                -lutilsd \
                -lqutilsd \
                -lUBXEnvd \
                -lubxactiondatad \
                -lUBXRobot3DViewd \
                -lmp3projd \
                -lUBXSocketSessiond \
                -L$${TRD}/libmpg123/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -llibmpg123 \
                -L$${TRD}/libsndfile/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -llibsndfile-1
    }
}

SOURCES += UBXMotionEditorImpl.cpp \
           UBXMotionEditor.cpp \
           stable.cpp \
           OptionsMotionWidget.cpp \
           UIMainLayerLayout.cpp \
           UBXMotionEditorLayout.cpp \
           UILayerManager.cpp \
           UILayerOptions.cpp \
           UIDialogBlockDescription.cpp \
           dialogcommandbox.cpp \
           OptionsEarLedWidget.cpp \
           mp3player.cpp \
           OptionsMusicWidget.cpp \
           OptionEyeLedWidget.cpp \
           LayerUI/ActionBlockData.cpp \
           LayerUI/ActionBlockDataManager.cpp \
           LayerUI/ActionBlockGroupManager.cpp \
           LayerUI/MotionDataPlay.cpp \
           LayerUI/EyeLedDataPlay.cpp \
           LayerUI/EarLedDataPlay.cpp \
           LayerUI/UILayerCommonWidget.cpp \
           LayerUI/UILayerCommonContainer.cpp \
           LayerUI/LayerCommonImpl.cpp \
           LayerUI/UIEyeLedLayer.cpp \
           LayerUI/UIMusicLayer.cpp \
           LayerUI/UIMotionLayer.cpp \
           LayerUI/EyeLedLayer.cpp \
           LayerUI/MotionLayer.cpp \
           LayerUI/MusicLayer.cpp \
           LayerUI/TimeLayer.cpp \
           LayerUI/UIEarLedLayer.cpp \
           LayerUI/EarLedLayer.cpp \
           LayerUI/UILayerImpl.cpp \
           LayerUI/DialogAddLayer.cpp \
           LayerUI/LayerControlWidget.cpp \
           LayerUI/GroupDataPlayThread.cpp \
           LayerUI/GroupDataManagerPlay.cpp \
           LayerUI/MusicDataPlay.cpp \
           LayerUI/UpdateTimeLineProcess.cpp \
           LayerUI/ExpressionLayer.cpp \
           LayerUI/ExpressionDataPlay.cpp \
           LayerUI/UIExpressionLayer.cpp \
           BatchModifyDialog.cpp \
           timesettingdialog.cpp \
           ledswidget.cpp

HEADERS += stable.h \
           UBXMotionEditorImpl.h \
           UBXMotionEditor.h \
           OptionsMotionWidget.h \
           UIMainLayerLayout.h \
           LayerUI/ILayerManager.h \
           LayerUI/ActionBlockData.h \
           LayerUI/ActionBlockDataManager.h \
           LayerUI/ActionBlockGroupManager.h \
           LayerUI/ILayerOptionHandler.h \
           LayerUI/MotionDataPlay.h \
           UBXMotionEditorLayout.h \
           LayerUI/ILayerUI.h \
           UILayerManager.h \
           LayerUI/ILayerOption.h \
           UILayerOptions.h \
           UIDialogBlockDescription.h \
           dialogcommandbox.h \
           LayerUI/IActionLayerManager.h \
           mp3projconstants.h \
           mp3player.h \
           OptionsMusicWidget.h \
           OptionEyeLedWidget.h \
           LayerUI/IUILayerManager.h \
           LayerUI/EyeLedDataPlay.h \
           LayerUI/EarLedDataPlay.h \
           ledswidget.h \
           LayerUI/UILayerCommonWidget.h \
           LayerUI/UILayerCommonContainer.h \
           LayerUI/ILayerCommonWidget.h \
           LayerUI/LayerCommonImpl.h \
           LayerUI/UIEarLedLayer.h \
           LayerUI/UIMotionLayer.h \
           LayerUI/UIMusicLayer.h \
           LayerUI/UIEyeLedLayer.h \
           LayerUI/EarLedLayer.h \
           LayerUI/EyeLedLayer.h \
           LayerUI/MotionLayer.h \
           LayerUI/MusicLayer.h \
           LayerUI/TimeLayer.h \
           OptionsEarLedWidget.h \
           timesettingdialog.h \
           LayerUI/UILayerImpl.h \
           LayerUI/DialogAddLayer.h \
           LayerUI/LayerControlWidget.h \
           LayerUI/GroupDataPlayThread.h \
           LayerUI/GroupDataManagerPlay.h \
           LayerUI/IGroupDataManagerPlay.h \
           LayerUI/MusicDataPlay.h \
           LayerUI/UpdateTimeLineProcess.h \
           BatchModifyDialog.h \
           LayerUI/ExpressionLayer.h \
           LayerUI/ExpressionDataPlay.h\
           LayerUI/UIExpressionLayer.h

FORMS += UIOptionMotionWidget.ui \
         UIMainLayerLayout.ui \
         UBXMotionEditorLayout.ui \
         UILayerManager.ui \
         UILayerOptions.ui \
         UIDialogBlockDescription.ui \
         dialogcommandbox.ui \
         OptionsEarLedWidget.ui \
         OptionsMusicWidget.ui \
         OptionEyeLedWidget.ui \
         ledswidget.ui \
         UILayerCommonContainer.ui \
         UILayerCommonWidget.ui \
         timesettingdialog.ui \
         DialogAddLayer.ui \
         LayerControlWidget.ui \
         BatchModifyDialog.ui

RESOURCES += res.qrc

TRANSLATIONS += $${SRC}/res/languages/UBXMotionEditor_zh_CN.ts \
                $${SRC}/res/languages/UBXMotionEditor_en.ts

#QMAKE_POST_LINK += \
#    rcc -binary $$PWD/res.qrc -o $$PWD/../res/rcc/UBXMotionEditor.rccs

#MOC_DIR         = $${TEMP}/$$TARGET/moc
#UI_DIR          = $${TEMP}/$$TARGET/ui
#UI_HEADERS_DIR  = $${TEMP}/$$TARGET/ui
#UI_SOURCES_DIR  = $${TEMP}/$$TARGET/ui
#OBJECTS_DIR     = $${TEMP}/$$TARGET/obj
#RCC_DIR         = $${TEMP}/$$TARGET/rcc
