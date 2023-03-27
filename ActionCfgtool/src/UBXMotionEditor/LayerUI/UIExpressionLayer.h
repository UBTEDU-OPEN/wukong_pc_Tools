
#ifndef _UIEXPRESSION_LAYER_H_
#define _UIEXPRESSION_LAYER_H_

#include "motioneditor_global.h"
#include "ILayerCommonWidget.h"
#include "UILayerImpl.h"

#pragma once
class MOTIONEDITOR_EXPORT CUIExpressionLayer : public CUILayerImpl
{
    Q_OBJECT
public:
    CUIExpressionLayer(ILayerCommonWidget* mp3Widget);

    virtual emActionLayerType GetType();
};

#endif