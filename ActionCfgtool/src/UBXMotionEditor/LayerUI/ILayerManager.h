#ifndef _ILAYERMANAGER_H_
#define _ILAYERMANAGER_H_

//#define ACTION_LAYER_TIME		0
//#define ACTION_LAYER_MOTION		1
//#define ACTION_LAYER_EYE		2
//#define ACTION_LAYER_EAR		3
//#define ACTION_LAYER_MUSIC		4

#define MAX_ACTION_LAYER		5

#include "motioneditor_global.h"
#include "stable.h"
class CIUBXSocketSession;
class ILayerOptionHandler;
class CActionBlockDataManager;
class ILayerUI;
class IUBXRobot3DView;
class ILibTreeNavigation;
class MOTIONEDITOR_EXPORT ILayerManager_Event
{
public:
    virtual void ILayerManager_SetLayOptionHandler(ILayerOptionHandler* pHandler) = 0;
    virtual void ILayerManager_SetMusicLayerSourceHandler(ILayerOptionHandler* pHandler , int nLayerType , void *pData , int nDataLen , int nDataID) = 0 ;
    virtual void ILayerManager_SetActionBlockDataManager(CActionBlockDataManager* pManager, int nCurrentGroupID,int nCurrentActionID, int nOffset = 0) = 0;
    virtual void ILayerManager_DeleteMusciFile(ILayerOptionHandler* pHandler) = 0;
    virtual void ILayerManager_LayerDebugStop(int layerType) = 0;
    virtual ILayerOptionHandler* ILayerManager_GetOptionHandler() = 0;
    virtual void ILayerManager_OnDeletOptionHandler(ILayerOptionHandler* pHandler) = 0;
    //设置控件的状态为bState
    virtual void ILayerManager_EnablePlay(int nLayerType , bool bState) = 0 ;

};

/************************************************************************/
/* layer管理类                                                                     */
/************************************************************************/
class MOTIONEDITOR_EXPORT ILayerManager
{
public:
    virtual bool Connect(QWidget* pWidget, ILayerManager_Event* pEvent) = 0;
    virtual void ReleaseConnection() = 0;
    virtual QWidget* GetWidget() = 0;

    virtual bool GetData(char** ppData, int &nLen) = 0;
    virtual bool SetData(char* pData, int nLen) = 0;
//    virtual void AdjustData() =0;
    virtual void LayerManager_GetUnUseLayers(int **pLayerID, int& nLayerCount) = 0;
    virtual QString LayerManager_GetLayerName(int nLayerID) = 0;
    virtual void LayerManager_AddLayer(int nLayerID) = 0;
//    virtual void LayerManager_AddMusic() = 0;
    virtual void LayerManager_ReleaseMusic() = 0;
//    virtual void LayerManager_DeleteLayer(int nLayerID) = 0;
    virtual ILayerUI* LayerManager_GetLayer(int LayerID) = 0;
    virtual void LayerManager_Set3DRobotViewInterface(IUBXRobot3DView* pRobotView) = 0;
    virtual void LayerManager_SetTimeScale(int timeScale) = 0;
    virtual int LayerManager_GetTimeScaleValue() = 0;
    virtual void LayerManager_SetTreeView(ILibTreeNavigation* pView) = 0;
//    virtual void LayerManager_TreeToView(char *pData, int nLen, QPoint* nPoint, QString FileName) = 0;
    virtual void LayerManager_SetMusicResource(QString strFilePath) = 0;
//    virtual void LayerManager_LayerPlaying(int layerType, ILayerOptionHandler* optionHandle) = 0;
    virtual void LayerManager_StopLayerDebug() = 0;
    virtual void LayerManager_LayerStop() = 0;
    virtual void LayerManager_SetSocketSession(CIUBXSocketSession* pSession) = 0;

    virtual void LayerManager_SwitchActionLayer(emActionLayerType eActionType) {};
//    virtual void LayerManager_UpdateScrollBar() = 0;
};

bool CreateLayerManager(ILayerManager** ppManager);
#endif  //_ILAYERMANAGER_H_
