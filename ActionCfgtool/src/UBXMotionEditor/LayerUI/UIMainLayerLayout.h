#ifndef CUIMainLayerLayout_H
#define CUIMainLayerLayout_H

#include "stable.h"
#include "LayerUI/ILayerManager.h"
#include "LayerUI/IUILayerManager.h"
#include "UIMainLayerLayout.h"
//#include "UIMainLayerLayoutToolsWidget.h"  //20160519万向峰合并代码时注释掉
#include "LayerUI/ILayerOptionHandler.h"

#include <QStyleOption>


namespace Ui {
class CUIMainLayerLayout;
}

class IUBXRobot3DView;
class CUIMainLayerLayout : public QWidget, public ILayerManager, public IUILayerManager_Event/*, public ILayoutTools_Event *///20160519万向峰合并代码时注释掉
{
    Q_OBJECT

public:
    explicit CUIMainLayerLayout(QWidget *parent = 0);
    ~CUIMainLayerLayout();
private:
    Ui::CUIMainLayerLayout *ui;

public:
    //IActionLayerManager 接口实现
    virtual bool Connect(QWidget* pParent, ILayerManager_Event* pEvent);
    virtual void ReleaseConnection();
    virtual QWidget* GetWidget();
    virtual bool GetData(char** ppData, int &nLen);
    virtual bool SetData(char* pData, int nLen);
//    virtual void AdjustData() ;
    virtual void LayerManager_GetUnUseLayers(int **pLayerID, int& nLayerCount);
    virtual QString LayerManager_GetLayerName(int nLayerID);
    virtual void LayerManager_AddLayer(int nLayerID);
//    virtual void LayerManager_AddMusic() ;
    virtual ILayerUI* LayerManager_GetLayer(int LayerID);
    virtual void LayerManager_ReleaseMusic() ;
//    virtual void LayerManager_DeleteLayer(int nLayerID);
    virtual void LayerManager_Set3DRobotViewInterface(IUBXRobot3DView* pRobotView);
    virtual void LayerManager_SetTreeView(ILibTreeNavigation* pView);
//    virtual void LayerManager_TreeToView(char *pData, int nLen, QPoint* nPoint, QString FileName);
    virtual void LayerManager_SetTimeScale(int timeScale);
    virtual int LayerManager_GetTimeScaleValue();
//    virtual void LayerManager_LayerPlaying(int layerType, ILayerOptionHandler* optionHandle);
    virtual void LayerManager_SetMusicResource(QString strFilePath);
    virtual void LayerManager_StopLayerDebug();
    virtual void LayerManager_LayerStop();
    virtual void LayerManager_SetSocketSession(CIUBXSocketSession* pSession);
//    virtual void LayerManager_UpdateScrollBar();

    // IUILayerManager_Event 接口实现
    virtual void LayerUIEvent_SelectChanger(int nCurrentCol, int nScale);
//    virtual void LayerUIEvent_AddLayerControl(int nLayerType);
    virtual void LayerUIEvent_SetLayOptionHandler(ILayerOptionHandler* pHandler);
    virtual void LayerUIEvent_SetMusicLayerSourceHandler(ILayerOptionHandler* pHandler, int nLayerType
                                                         , void *pData , int nDataLen , int nDataID);
    virtual void LayerUIEvent_DeleteMusciFile(ILayerOptionHandler* pHandler) ;
    virtual void LayerUIEvent_SetActionBlockDataManager(CActionBlockDataManager* pManager, int nCurrentGroupID,int nCurrentActionID, int nOffset = 0);
//    virtual void LayerUIEvent_UpdateScrollBar();
//    virtual void LayerUIEvent_SetScrollBarPos(int nPos);
    virtual void LayerUIEvent_OnDeletOptionHandler(ILayerOptionHandler* pHandler);
    virtual void LayerUIEvent_SetScaleValue(int nScale);
    virtual void LayerDebugStop(int layerType);
    virtual ILayerOptionHandler* LayerUIEvent_GetOptionHandler();

    /**************************************************************************
    * 函数名：LayerUIEvent_MusicHasLoaded
    * 功能: 音乐加载完成事件通知
    * 参数:
    *    @[in ] strMp3Path: 音乐路径
    *    @[in ] strTitle: 音乐标题
    *    @[in ] nTime: 音乐总时长
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    virtual void LayerUIEvent_MusicHasLoaded(const QString& strMp3Path, const QString& strTitle, int nTime);

public:
    // ILayoutTools_Event
    //        virtual void OnSrcollChange(int nPos);
    //        virtual void EnablePlay(int nLayerType ,bool bState) ;

protected:
    //If you subclass from QWidget, you need to provide a paintEvent for your custom QWidget as below:
    void paintEvent(QPaintEvent *);

private:
    IUILayerManager*	m_pUILayerManager;
    //        CUIMainLayerLayoutToolsWidget	m_toolsWidget;  //20160519万向峰合并代码时注释掉
    ILayerManager_Event*  m_pEvent;
    ILayerOptionHandler* m_pMotionWidget;
    //      CIUBXSocketSession* m_pSocketSession;
    int m_StopDebugCount;

};

#endif // CUIMainLayerLayout_H
