#pragma once

#include "motioneditor_global.h"
#include "stable.h"
/************************************************************************/
/* layerui 事件通知类                                                                     */
/************************************************************************/
class LayerUI_Event
{
public:
};

class ILayerOptionHandler;
class CActionBlockDataManager;
class MOTIONEDITOR_EXPORT IUILayerManager_Event
{
public:
    // 选择不同的列表
    virtual void LayerUIEvent_SelectChanger(int nCurrentCol, int nScale) = 0;
    //	virtual void LayerUIEvent_AddLayerControl(int nLayerType) = 0;
    virtual void LayerUIEvent_SetLayOptionHandler(ILayerOptionHandler* pHandler) = 0;
    virtual void LayerUIEvent_SetMusicLayerSourceHandler(ILayerOptionHandler* pHandler , int nLayerType
                                                         , void *pData , int nDataLen , int nDataID) = 0;
    virtual void LayerUIEvent_SetScaleValue(int nScale)=0;
    virtual void LayerUIEvent_DeleteMusciFile(ILayerOptionHandler* pHandler) = 0;
    virtual void LayerUIEvent_SetActionBlockDataManager(CActionBlockDataManager* pManager, int nCurrentGroupID,int nCurrentActionID = -1, int nOffset = 0) = 0;
    //	virtual void LayerUIEvent_UpdateScrollBar() = 0;
    //	virtual void LayerUIEvent_SetScrollBarPos(int nPos) = 0;
    virtual void LayerUIEvent_OnDeletOptionHandler(ILayerOptionHandler* pHandler)=0;
    //音乐加载完成事件通知
    virtual void LayerUIEvent_MusicHasLoaded(const QString& strMp3Path, const QString& strTitle, int nTime) = 0;

    virtual void LayerDebugStop(int layerType) = 0;
    //    virtual void EnablePlay(int nLayerType ,bool bState) = 0;2016/04.10万向峰合并代码时注释掉
    virtual ILayerOptionHandler* LayerUIEvent_GetOptionHandler() = 0;
};

class CIUBXSocketSession;
class ILayerUI;
class CActionBlockGroupManager;
class CActionBlockData;
class IUBXRobot3DView;
class ILibTreeNavigation;
/************************************************************************/
/* Layer 层管理类接口                                                                    */
/************************************************************************/
class IUILayerManager: public LayerUI_Event
{
public:
    // 初始化
    virtual bool Connect(QWidget* pWnd, IUILayerManager_Event* pEvent) = 0;
    // 释放资源
    virtual void ReleaseConnection() = 0;

    virtual QWidget* GetWidget() = 0;
    //	virtual void OnDraw(CDC* pDC) = 0;
    virtual bool SetData(char* pData, int nLen) = 0;
    //    virtual void AdjustData() =0;
    virtual bool GetData(char** ppData, int &nLen) = 0;
    //	virtual void SetPosition(int PositionX, int PositionY) = 0;
    virtual bool LoadMusic(const QString& strFilePath) = 0;
    virtual void ReleaseMusic() = 0;
    virtual bool AddActionLayer(ILayerUI* pLayer) = 0;
    virtual bool AddAcionLayer(int nLayerID) = 0;
    // 查找层
    virtual ILayerUI* FindLayer(int nTypeID) = 0;
    // 删除层
    virtual bool DeleteActionLayer(int nTypeID) = 0;
    // 删除所有层
    virtual void DeleteAllLayer() = 0;
    // 获取LAY数量
    virtual int GetLayerCount() = 0;
    //功能:根据Layer序列获取层类型
    virtual emActionLayerType GetLayerTypeByIndex(int nIndex) = 0;
    // 根据序号查找层
    virtual ILayerUI* FindLayerByIndex(int nIndex) = 0;

    //leo add 20170228
    virtual void TreeToView( char *pData, int nLen, emActionLayerType eType, int nCur, QString FileName ) = 0;

    // 滚动条改变处理
    //    virtual void SetHScrollOffset(uint nPos) = 0;
    //滚动条
    //    virtual void SetHScrollPos(int nCol) = 0;
    // 获取层名
    virtual QString GetLayerName(int nLayerID) = 0;
    // 获取当前选中的时间点
    virtual int GetCurrentSelTime() = 0;
    //
    virtual int SetCurrentSelectCol(int nCol) = 0;
    virtual void SetActionDataGroupManagerToOptions(CActionBlockDataManager* pGroupManager, int nCurrentGroupID , int nCurrentActionID, int nOffset = 0 ) = 0;
    //virtual void SetActionToOptions(int nCurrentGroupID , int nCurrentActionID) = 0;
    virtual void Set3DRobotViewInterface(IUBXRobot3DView* pRobotView) = 0;
    virtual IUBXRobot3DView* Get3DRobotViewInterface() = 0;
    virtual void SetTreeView(ILibTreeNavigation* pView) = 0;

    // 更新滚动条
    //    virtual void UpdateScrollBar() = 0;
    // 根据给点的时间块，得出最大的时间
    virtual int GetNewMaxCellCount(int nMax) = 0;

    virtual void SetTimeScaleValues(int timeScale) = 0;
    virtual void FreshTimeScaleValues(int timeSacle) = 0;
    virtual int GetTimeScaleValues() = 0;
    virtual int GetLineThick() = 0;
    virtual void SetLineThick(int nThick) = 0;
    virtual int GetCellWidth() = 0;
    virtual void SetCellWidth(int nWidth) = 0;
    virtual int GetCellCount() = 0;
    virtual void SetCellCount(int nCount) = 0;
    virtual int GetCurrentSelectCol() = 0;

    virtual void LayerPlaying(int layerType, ILayerOptionHandler* optionHandle) = 0;
    virtual void StopLayerDebug() = 0;
    virtual void LayerStop() = 0;
    virtual void TotalLayerPlaying() = 0;
    virtual void StopTotalLayerDebug() = 0;
    virtual void PauseTotalLayerDebug() = 0;
    virtual void SetOriginPos(int nCol){}
    virtual ILayerOptionHandler* GetOptionHandler() = 0;
    virtual void IUILayerManager_SetSocketSession(CIUBXSocketSession* pSession) = 0;
    virtual void IUILayerManager_SwitchLayer(emActionLayerType eActionType) {};
    virtual void IUILayerManager_SetMusicFile(int nLayerType , void *pData , int nDataLen , int nDataID) = 0;
    virtual void IUILayerManager_DeleteMusicFile(int nLayerType) = 0;
    virtual void IUILayerManager_SetModifiedState(bool bState) = 0;
    virtual bool IUILayerManager_GetModifiedState() = 0;
    virtual bool IUILayerManager_IsRobotDebuging() = 0;
    virtual void IUILayerManager_SingleBlockPlaying(int nLayerType ,CActionBlockData *pActionData) = 0;
    virtual void IUILayerManager_SingleGroupPlaying(int nLayerType , CActionBlockGroupManager* pGroupManager) = 0;
    virtual void IUILayerManager_SingleBlockPlayingStop(int nLayerType) = 0;
    //检测音乐是否有更新
    //    virtual void IUILayerManager_SetMusicModifiedState(bool bState) = 0;
    //    virtual bool IUILayerManager_GetMusicModifiedState() = 0;
    //Joker 通知Layer模块更新界面
    virtual void UpdateLayerWidget(CActionBlockDataManager* pGroupManager, int nCurrentGroupID , int nCurrentActionID, emActionLayerType nLayerType) = 0;
    virtual void UpdateLayerAction(int nGroupID, int nStartTime, emActionLayerType nType) = 0;
    virtual void HasMotionData(bool hasData, int nLayerType){}//通知MP3是否有动作帧，以便刷新按钮状态

    virtual void FinishMusicPlaying() = 0;//音乐播放完成时通知

    virtual void LayerManager_OnLButtonDown(int layerType) = 0;

    //Layer层通知更新LayerManager
    virtual void OnLayerLButtonDown(emActionLayerType nLayerType, int nCol) = 0;
    virtual void OnLayerRButtonDown(emActionLayerType nLayerType, int nCol) = 0;
    virtual void OnLayerMouseMove(emActionLayerType nLayerType , int nCol) = 0;

    virtual void OnDoMenuCommand(emActionLayerType nLayerType, int nCommond) =0;
    //音乐加载完成事件通知
    virtual void IUILayerManager_MusicHasLoaded(const QString& strMp3Path, const QString& strTitle, int nTime) = 0;
    //停止眼睛灯仿真
    virtual void IUILayerManager_StopPlayEyeData() = 0;
    //停止耳朵灯仿真
    virtual void IUILayerManager_StopPlayEarData() = 0;

    // 表情帧显示
    virtual void IUILayerManager_ShowExpression(QString strFileName, int nCurFrame) = 0;
};

bool CreateUILayerManager(IUILayerManager** mManager);
