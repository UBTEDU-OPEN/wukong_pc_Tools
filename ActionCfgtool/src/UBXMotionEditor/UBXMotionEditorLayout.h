#ifndef UBXMOTIONEDITORLAYOUT_H
#define UBXMOTIONEDITORLAYOUT_H

#include "motioneditor_global.h"
#include "stable.h"
#include "LayerUI/iActionLayerManager.h"
#include "LayerUI/ILayerManager.h"
//#include "LayerUI/ILayerControl.h"
#include "LayerUI/ILayerOption.h"

#include <QStyleOption>

class IUBXRobot3DView;
namespace Ui {
class CUBXMotionEditorLayout;
}

class MOTIONEDITOR_EXPORT CUBXMotionEditorLayout : public QWidget,public IActionLayerManager,public ILayerManager_Event/*, public ILayerControlEvent*/
{
    Q_OBJECT

public:
    explicit CUBXMotionEditorLayout(QWidget *parent = 0);
    ~CUBXMotionEditorLayout();

private:
    Ui::CUBXMotionEditorLayout *ui;

private:
    ILayerManager*			m_pMainLayer;
//    ILayerControl*			m_pLayerControl;
    ILayerOption*			m_pLayerOption;
//    CXSplitterWnd			m_SplitterWnd;
    CIUBXSocketSession*		m_pSocketSession;
    bool                    m_bModified;

public:
    // IActionLayerManager接口实现
    virtual bool Connect(QWidget* pParent);
    virtual void ReleaseConnection();
    virtual QWidget* GetWidget() {return this;}
    virtual bool GetData(char** ppData, int &nLen);
    virtual bool SetData(char* pData, int nLen);
//    virtual void AdjustData();
    virtual void ActionLayerManager_Set3DRobotViewInterface(IUBXRobot3DView* pRobotView);
    virtual void ActionLayerManager_SetSocketSession(CIUBXSocketSession* pSession);
    virtual void ActionLayerManager_SetMusicResource(QString strFilePath);
//    virtual void ActionLayerManager_UpdateScrollBar();
    virtual void ActionLayerManager_StopDebug();
    virtual void ActionLayerManager_ReleaseMusicSource() ;
    virtual void ActionLayerManager_SetTreeView(ILibTreeNavigation* pView);
//    virtual void ActionLayerManager_TreeToView(char *pData, int nLen, QPoint* nPoint, QString FileName);
    virtual bool ActionLayerManager_IsFileModified();
    virtual bool ActionLayerManager_IsRobotDebuging() ;
    virtual void ActionLayerManager_SetFileModified(bool bState);

    virtual void ActionLayerManager_SwitchActionLayer(emActionLayerType eActionType);
//    virtual bool ActionLayerManager_IsMusicModified();
//    virtual void ActionLayerManager_SetMusicModified(bool bState);
public:
    // ILayerControlEvent 接口实现
//    virtual void ILayerControlEvent_GetUnUseLayers(int **pLayerID, int& nLayerCount);
//    virtual QString ILayerControlEvent_GetLayerName(int nLayerID);
//    virtual void ILayerControlEvent_AddLayer(int nLayerID);
//    virtual void ILayerControlEvent_AddLayerControl(int nLayerType);
//    virtual void LayerControlEvent_DeleteLayer(int nLayerID);
//    virtual void LayerControlEvent_SetTimeScaleValue(int timeScale);
//    virtual int LayerControlEvent_GetTimeScaleValue();

//    virtual void LayerControlEvent_LayerPlaying(int layerType);
//    virtual void LayerControlEvent_AddMusic() ;
//    virtual void LayerControlEvent_StopLayerDebug();
//    virtual void LayerControlEvent_LayerStop() ;
//    virtual void LayerControlEvent_SetCurrentOption(int layerType);
//    virtual void LayerControlEvent_SetMusicSource(int nLayerType , void *pData , int nDataLen , int nDataID);

public:
    // ILayerManager_Event 接口实现
    virtual void ILayerManager_SetLayOptionHandler(ILayerOptionHandler* pHandler);
    virtual void ILayerManager_SetMusicLayerSourceHandler(ILayerOptionHandler* pHandler , int nLayerType , void *pData , int nDataLen , int nDataID);
    virtual void ILayerManager_SetActionBlockDataManager(CActionBlockDataManager* pManager, int nCurrentGroupID,int nCurrentActionID, int nOffset = 0);

    virtual void ILayerManager_LayerDebugStop(int layerType);
    virtual void ILayerManager_DeleteMusciFile(ILayerOptionHandler* pHandler) ;
    virtual ILayerOptionHandler* ILayerManager_GetOptionHandler();
    virtual void ILayerManager_OnDeletOptionHandler(ILayerOptionHandler* pHandler);
    virtual void ILayerManager_EnablePlay(int nLayerType ,bool bState)  ;
    //virtual void ILayerManager_EnablePlay(int nLayerType , bool bState)  ;

protected:
    //If you subclass from QWidget, you need to provide a paintEvent for your custom QWidget as below:
    void paintEvent(QPaintEvent *);

private:
    void setDefaultStyle();
};

#endif // UBXMOTIONEDITORLAYOUT_H
