#ifndef _UITIMELAYER_H_
#define _UITIMELAYER_H_

#include "motioneditor_global.h"
#include "ILayerUI.h"
#include "LayerCommonImpl.h"

/************************************************************************/
/* 时间层实现类                                                                     */
/************************************************************************/
class CLayerCommonImpl;
class MOTIONEDITOR_EXPORT CTimeLayer: public CLayerCommonImpl
{
public:
    CTimeLayer(void);
    ~CTimeLayer(void);

public:
    virtual bool Connect(QWidget* managerHandle);
    virtual bool ReleaseConnection();

    virtual bool GetData(char** ppData, int& nLen);
    virtual bool SetData(char* pData, int nLen);
    //取得类型，具体见宏定义
    virtual emActionLayerType GetType();
    virtual QString GetLayerName();

private:
    QWidget* m_ManagerHandle;

};
#endif  //_UITIMELAYER_H_
