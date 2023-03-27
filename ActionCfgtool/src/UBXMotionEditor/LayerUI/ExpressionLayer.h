
#ifndef _EXPRESSIONLAYER_H
#define _EXPRESSIONLAYER_H

#include "motioneditor_global.h"
#include "LayerCommonImpl.h"

class MOTIONEDITOR_EXPORT CExpressionLayer : public CLayerCommonImpl
{
public:
    CExpressionLayer();
    ~CExpressionLayer();

public:
    virtual emActionLayerType GetType();
    virtual QString GetLayerName();
    virtual void CreateOptionHandler();
};

#endif