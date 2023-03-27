#pragma once
#include "motioneditor_global.h"
#include "robotdef.h"
class CIUBXSocketSession;
class IUBXRobot3DView;
class ILayerUI;
class IUILayerManager;
class MOTIONEDITOR_EXPORT IGroupDataManagerPlay_Event
{
public:
	virtual void LayerDebugStop(int layerType) = 0;
    virtual void PlayEvent_OnSetPos(int nCol) = 0;
    virtual void PlayEvent_PlayMp3() = 0;
};
/************************************************************************/
/* 层数据播放接口                                                                     */
/************************************************************************/
class MOTIONEDITOR_EXPORT IGroupDataManagerPlay
{
public: 
    /**************************************************************************
    * 函数名：Connection
    * 功能: 建立联系，初始化必要的参数
    * 参数:
    *    @[in ] pDataManager: LayerUI管理者
    *    @[in ] pEvent: 仿真事件响应者
    *    @[in ] PlayHandle: 仿真执行句柄
    *    @[in ] bSingleLayer: 是否为单层仿真
    * 返回值: 无
    * 时间: 2017/2/26
    * 作者: wanxf
    */
    virtual bool Connection(ILayerUI* pDataManager, IGroupDataManagerPlay_Event* pEvent, void* PlayHandle, bool bSingleLayer) = 0;
	virtual void ReleaseConnection() = 0;

	// 开始播放
	virtual void StartPlay() = 0;
	// 正在播放的时候步进值，每过一个时间粒度调用一次
	virtual bool OnPlaying(int nOffsetTime) = 0;
	// 停止播放
	virtual void StopPlay() = 0;
    // 暂停播放
    virtual void PausePlay(emActionLayerType nType) = 0;
    //恢复继续播放
    virtual void OnResumePlay(emActionLayerType nType) = 0;

//    virtual void IGroupDataManagerPlay_SetRobot3DView(IUBXRobot3DView* pSession) = 0;
    virtual void IGroupDataManagerPlay_SetLayerManager(IUILayerManager* pLayerManager) = 0;
    virtual void IGroupDataManagerPlay_SetSocketSession(CIUBXSocketSession* pSession) = 0;

};

bool CreateIGroupDataManagerPlay(IGroupDataManagerPlay** ppManagerPlay);
