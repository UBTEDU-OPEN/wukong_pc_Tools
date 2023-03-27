#ifndef UILAYEROPTIONS_H
#define UILAYEROPTIONS_H

#include "motioneditor_global.h"
//#include "stable.h"
#include "LayerUI/ILayerOption.h"

namespace Ui {
class CUILayerOptions;
}

class CActionBlockDataManager;
class MOTIONEDITOR_EXPORT CUILayerOptions : public QWidget, public ILayerOption
{
    Q_OBJECT

public:
    explicit CUILayerOptions(QWidget *parent = 0);
    ~CUILayerOptions();

private:
    Ui::CUILayerOptions *ui;
private:
    ILayerOptionHandler* m_pOptionHandler;
    CActionBlockDataManager* m_pGroupManager;

public:
    // ILayerOption接口实现
    virtual bool Connect(QWidget* pParent);
    virtual void ReleaseConnection();
    virtual QWidget* GetWidget();
    virtual void SetOptionHandler(ILayerOptionHandler* pHandler);
    virtual void SetMusicOptionSources(ILayerOptionHandler* pHandler , int nLayerType , void *pData , int nDataLen , int nDataID);
    virtual void DeleteMusicResource(ILayerOptionHandler* pHandler);
    virtual ILayerOptionHandler* GetOptionHandler();
//    virtual ILayerOptionHandler* GetOptionHandler() = 0;
    virtual void SetActionBlockDataManager(CActionBlockDataManager* pManager, int nCurrentGroupID,int nCurrentActionID, int nOffset = 0);
    virtual void ResetState();
    virtual void OnDeletOptionHandler(ILayerOptionHandler* pHandler);
};

#endif // UILAYEROPTIONS_H
