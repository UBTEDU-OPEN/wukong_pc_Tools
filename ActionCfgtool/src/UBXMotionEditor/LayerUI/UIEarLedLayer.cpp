#include "UIEarLedLayer.h"
#include "ActionBlockGroupManager.h"
#include "ActionBlockData.h"
#include "mp3projconstants.h"
//#include "mp3widget.h"
#include <QPixmap>

CUIEarLedLayer::CUIEarLedLayer(ILayerCommonWidget *mp3Widget)
    :CUILayerImpl(mp3Widget)
{
}

//获取层类型
emActionLayerType CUIEarLedLayer::GetType()
{
    return ACTION_LAYER_EAR;
}

