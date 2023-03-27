#pragma once

/************************************************************************/
/* 组数据播放接口                                                                     */
/************************************************************************/
#define PLAYDEBUG_ALL 0
#define PLAYDEBUG_GROUP 1
#define PLAYDEBUG_BLOCK 2

class CActionBlockGroupManager;
class CLayerCommonImpl;
class IUILayerManager;
class CIUBXSocketSession;
class IGroupDataPlay
{
public:
    virtual bool Connect(CActionBlockGroupManager* pGroupManager, bool bSinglePlay, int nScaleTime) = 0;
    virtual void ReleaseConnection() = 0;
    virtual void StartPlay(int nEndTime, void* PlayHandle , int nBlockIndex = 0, int nTimeOffset = 1) = 0;
    virtual void PausePlay() = 0;
    virtual void ResumePlay() = 0;
    virtual void StopPlay() = 0;

    virtual bool OnPlaying(int nOffset) = 0;
//    virtual void IGroupDataPlay_SetRobot3dView(IUBXRobot3DView* p3DView) = 0;
//    virtual void IGroupDataPlay_SetLayerManeger(IUILayerManager* pLayerManager) = 0;
//    virtual void IGroupDataPlay_SetSocketSession(CIUBXSocketSession* pSession) = 0;
};

