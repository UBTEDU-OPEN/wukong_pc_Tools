
#include "stable.h"
#include "UIMotionLayer.h"
#include "ActionBlockGroupManager.h"
#include "ActionBlockData.h"
#include "mp3projconstants.h"
//#include "mp3widget.h"
#include <QPixmap>


CUIMotionLayer::CUIMotionLayer(ILayerCommonWidget *mp3Widget)
    :CUILayerImpl(mp3Widget)
{
}

//获取层类型
emActionLayerType CUIMotionLayer::GetType()
{
    return ACTION_LAYER_MOTION;
}


// 头部层
CUIHeadMotionLayer::CUIHeadMotionLayer(ILayerCommonWidget *mp3Widget)
    :CUILayerImpl(mp3Widget)
{
}

emActionLayerType CUIHeadMotionLayer::GetType()
{
    return ACTION_LAYER_HEAD;
}



// 手部层
CUIHandMotionLayer::CUIHandMotionLayer(ILayerCommonWidget *mp3Widget)
    :CUILayerImpl(mp3Widget)
{
}

emActionLayerType CUIHandMotionLayer::GetType()
{
    return ACTION_LAYER_HAND;
}



// 腿部层
CUILegMotionLayer::CUILegMotionLayer(ILayerCommonWidget *mp3Widget)
    :CUILayerImpl(mp3Widget)
{
}

emActionLayerType CUILegMotionLayer::GetType()
{
    return ACTION_LAYER_LEG;
}