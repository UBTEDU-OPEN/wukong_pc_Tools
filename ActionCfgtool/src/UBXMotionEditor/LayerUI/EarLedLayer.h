#ifndef CUIEARLEDLAYER_H
#define CUIEARLEDLAYER_H

#include "motioneditor_global.h"
#include "LayerCommonImpl.h"

class MOTIONEDITOR_EXPORT CEarLedLayer:public CLayerCommonImpl
{
public:
    CEarLedLayer(void);
    ~CEarLedLayer(void);

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

#endif // CUIEARLEDLAYER_H
