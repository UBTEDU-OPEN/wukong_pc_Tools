#ifndef _ILAYEROPTION_H_
#define _ILAYEROPTION_H_

#include "motioneditor_global.h"
#include "stable.h"
class ILayerOptionHandler;
class CActionBlockDataManager;
class MOTIONEDITOR_EXPORT ILayerOption
{
public:
    virtual bool Connect(QWidget* pParent) = 0;
    virtual void ReleaseConnection() = 0;
    virtual QWidget* GetWidget() = 0;
    virtual void SetOptionHandler(ILayerOptionHandler* pHandler) = 0;
    virtual void SetMusicOptionSources(ILayerOptionHandler* pHandler , int nLayerType , void *pData , int nDataLen , int nDataID) = 0;
    virtual void DeleteMusicResource(ILayerOptionHandler* pHandler) = 0;
    virtual ILayerOptionHandler* GetOptionHandler() = 0;
    virtual void SetActionBlockDataManager(CActionBlockDataManager* pManager, int nCurrentGroupID,int nCurrentActionID = -1, int nOffset = 0) = 0;
    // 重置状态
    virtual void ResetState() = 0;
    virtual void OnDeletOptionHandler(ILayerOptionHandler* pHandler) = 0;
};

bool CreateLayerOption(ILayerOption** ppOption);
#endif  //_ILAYEROPTION_H_
