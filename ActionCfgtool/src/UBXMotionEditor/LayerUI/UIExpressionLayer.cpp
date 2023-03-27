
#include "UIExpressionLayer.h"



CUIExpressionLayer::CUIExpressionLayer(ILayerCommonWidget *mp3Widget)
    : CUILayerImpl(mp3Widget)
{

}


emActionLayerType CUIExpressionLayer::GetType()
{
    return ACTION_LAYER_EXPRESSION;
}
