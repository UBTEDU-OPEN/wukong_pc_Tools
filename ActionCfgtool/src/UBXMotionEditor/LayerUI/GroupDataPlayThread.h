#pragma once

#include "motioneditor_global.h"
#include "IGroupDataPlay.h"
#include "LayerCommonImpl.h"
//#include "IGroupDataManagerPlay.h"
#include <QThread>

class CActionBlockData;
/************************************************************************/
/* 播放处理类                                                                     */
/************************************************************************/
class MOTIONEDITOR_EXPORT CActionBlockProcess
{
public:
    /**************************************************************************
    * 函数名：ProcessPlayActionBlock
    * 功能: 对组里的每一个具体块进行仿真
    * 参数:
    *    @[in ] pActionBlock: 仿真的块数据
    *    @[in ] nRunTime: 乘以时间刻度后的运行时间
    *    @[in ] TimeScale: 时间刻度值
    * 返回值: 无
    * 时间: 2017/2/26
    * 作者: wanxf
    */
    virtual void ProcessPlayActionBlock(CActionBlockData* pActionBlock, int nRunTime, int TimeScale , bool bEnd = false) = 0;
    virtual void OnStopPlay() = 0;
    virtual void StopPlay() = 0;
};

class CActionBlockData;
class CGroupDataPlayThread : public QThread, public IGroupDataPlay
{
public:
    CGroupDataPlayThread(void);
    ~CGroupDataPlayThread(void);

public:
    // IGroupDataPlay接口实现
    virtual bool Connect(CActionBlockGroupManager* pGroupManager, bool bSinglePlay, int nScaleTime);
    virtual void ReleaseConnection();
    virtual void StartPlay(int nEndTime, void* PlayHandle , int nBlockIndex = 0, int nTimeOffset = 1);
    virtual void PausePlay();
    virtual void ResumePlay();
    virtual void StopPlay();
    virtual bool OnPlaying(int nOffset);
//    virtual void IGroupDataPlay_SetRobot3dView(IUBXRobot3DView* pSession);
//    virtual void IGroupDataPlay_SetLayerManeger(IUILayerManager* pLayerManager);

private:
    bool m_bPause;
    bool m_bStop;
    bool m_bSinglePlay;

    CActionBlockGroupManager* m_pGroupManager;
    // 当前运行的总时间
    int	m_nCurrentTime;
    // 是否需要播放下一块的比较时间
    int m_nCurrentCompareTime;
    // 当前块的总时间
    int m_nTotalTime;
    // 当前暂停的时间
    int m_nPauseTime;
    // 当前播放的动作块
    CActionBlockData* m_pCurrentActionBlockPlay;
    // 已经执行过的块
    int m_nPastIndex;
    // 时间粒度
    int m_nCurrentScaleTime;
    CActionBlockProcess* m_pProcess;

    int m_nScaleTime;

    void resetState();

protected:
//    IUBXRobot3DView* m_pRobot3DView;
    IUILayerManager* m_pUILayerManager;

public:
    void SetProcessHander(CActionBlockProcess* pProcess);
    void run();
};

