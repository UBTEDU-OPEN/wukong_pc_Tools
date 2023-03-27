#ifndef EYELEDLAYER_H
#define EYELEDLAYER_H
#include "motioneditor_global.h"
#include "ActionBlockDataManager.h"
#include "ILayerCommonWidget.h"
#include "UILayerImpl.h"
//#include <QObject>

//class CUILayerCommonWidget;
class MOTIONEDITOR_EXPORT CUIEyeLedLayer : public CUILayerImpl
{
    Q_OBJECT
public:
    CUIEyeLedLayer(ILayerCommonWidget* mp3Widget);

public:
    //获取层类型
    virtual emActionLayerType GetType();
};

#endif // EYELEDLAYER_H
