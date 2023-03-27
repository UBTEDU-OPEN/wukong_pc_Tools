#ifndef EARLEDLAYER_H
#define EARLEDLAYER_H

#include "motioneditor_global.h"
#include "ActionBlockDataManager.h"
#include "ILayerCommonWidget.h"
#include "UILayerImpl.h"
//#include <QObject>

//class CUILayerCommonWidget;
class MOTIONEDITOR_EXPORT CUIEarLedLayer : public CUILayerImpl/*, public QObject*/
{
    Q_OBJECT

public:
    CUIEarLedLayer(ILayerCommonWidget *mp3Widget);
public:
    //获取层类型
    virtual emActionLayerType GetType();
};

#endif // EARLEDLAYER_H
