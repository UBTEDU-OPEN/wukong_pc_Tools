#ifndef UIEYELEDLAYER_H
#define UIEYELEDLAYER_H

#include "motioneditor_global.h"
#include "LayerCommonImpl.h"
#include "QRgb"

class MOTIONEDITOR_EXPORT CEyeLedLayer:public CLayerCommonImpl
{
public:
    CEyeLedLayer(void);
    ~CEyeLedLayer(void);

public:
//    virtual bool Connect(QWidget* managerHandle);
//    virtual bool ReleaseConnection();

//    virtual bool GetData(char** ppData, int& nLen);
//    virtual bool SetData(char* pData, int nLen);
    // 取得类型，具体见宏定义
    virtual emActionLayerType GetType();
    virtual QString GetLayerName();
    virtual void CreateOptionHandler();
};

#endif // UIEYELEDLAYER_H
