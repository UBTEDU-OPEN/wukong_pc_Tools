﻿#ifndef _UIMUSICLAYER_H_
#define _UIMUSICLAYER_H_

#include "motioneditor_global.h"
#include "LayerCommonImpl.h"
class MOTIONEDITOR_EXPORT CMusicLayer: public CLayerCommonImpl
{
public:
    CMusicLayer(void);
    ~CMusicLayer(void);

public:
    //ILayerUI接口实现
//    virtual bool Connect(QWidget* managerHandle);
//    virtual bool ReleaseConnection();

    virtual bool GetData(char** ppData, int& nLen);
    virtual bool SetData(char* pData, int nLen);
    // 取得类型，具体见宏定义
    virtual emActionLayerType GetType();
    virtual QString GetLayerName();
    virtual void CreateOptionHandler();
private:
    bool AddMusicFromMusicDir();
    void AddMusic(QString strMusicFile);
};
#endif  //_UIMUSICLAYER_H_
