#include "UIEyeLedLayer.h"
#include "ActionBlockGroupManager.h"
#include "ActionBlockData.h"
#include "mp3projconstants.h"
#include "UILayerCommonWidget.h"
#include <QPixmap>

CUIEyeLedLayer::CUIEyeLedLayer(ILayerCommonWidget *mp3Widget)
    :CUILayerImpl(mp3Widget)
{

}

emActionLayerType CUIEyeLedLayer::GetType()
{
    return ACTION_LAYER_EYE;
}
