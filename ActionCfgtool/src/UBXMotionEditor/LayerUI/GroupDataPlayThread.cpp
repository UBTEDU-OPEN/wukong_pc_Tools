#include "stable.h"
#include "GroupDataPlayThread.h"
#include "ActionBlockGroupManager.h"
#include "ActionBlockData.h"
#include <QDebug>

CGroupDataPlayThread::CGroupDataPlayThread(void)
: m_pGroupManager(NULL)
, m_pProcess(NULL)
, m_nPastIndex(0)
, m_nCurrentCompareTime(0)
, m_bPause(false)
, m_bStop(false)
{

}

CGroupDataPlayThread::~CGroupDataPlayThread(void)
{
    StopPlay();
}

void CGroupDataPlayThread::resetState()
{
    m_nPastIndex = 0;
    m_nCurrentTime = 0;

    m_bPause = false;
    m_bStop = false;
}

void CGroupDataPlayThread::run()
{
    if (m_bSinglePlay == false)
        return;

    while (m_bStop == false)
    {
        if (m_bPause)
        {
            continue;
        }

        msleep(m_nScaleTime);
        if (!OnPlaying(1))
        {
            break;
        }
    }

    resetState();
}

bool CGroupDataPlayThread::Connect(CActionBlockGroupManager* pGroupManager, bool bSinglePlay, int nScaleTime)
{
    wait(100);
    m_bPause = false;
    m_bStop = false;
    m_bSinglePlay = bSinglePlay;
    m_pGroupManager = pGroupManager;
    m_nScaleTime = nScaleTime;

    return true;
}

void CGroupDataPlayThread::ReleaseConnection()
{
    m_bStop = true;
    wait(500);
    delete this;
}

void CGroupDataPlayThread::StartPlay(int nEndTime, void* PlayHandle , int nBlockIndex , int nTimeOffset)
{
    m_nTotalTime = nEndTime;

    // 如果当前组没有数据
    if (m_pGroupManager->GetActionCount() == 0)
    {
        return;
    }

    // Modify by jianjie 2017/11/24
    // 鼠标所在当前帧不播放，时间残留仍需计算
    m_nPastIndex    = nBlockIndex;
    m_nCurrentTime  = nTimeOffset - 1;      // 当前已执行时间 = 偏移时间 - 1

    // 动作块序号从0开始，大于-1在组内，要执行动作
    if (nBlockIndex > -1)
    {
        m_pCurrentActionBlockPlay = m_pGroupManager->GetAt(nBlockIndex);

        int nRunTime = m_pCurrentActionBlockPlay->GetRunTime();
        m_nCurrentCompareTime = nRunTime + m_pCurrentActionBlockPlay->GetStopTime();

        // 未执行的时间继续执行
        if (m_pProcess)
        {
            bool bEnd = m_pGroupManager->GetActionCount() == 1 ? true : false;
            m_pProcess->ProcessPlayActionBlock(m_pCurrentActionBlockPlay, (nRunTime - m_nCurrentTime)*m_nScaleTime, m_nScaleTime, bEnd);
        }
    }

    if (m_bSinglePlay)
    {
        start();
    }
}

void CGroupDataPlayThread::SetProcessHander(CActionBlockProcess* pProcess)
{
    m_pProcess = pProcess;
}

void CGroupDataPlayThread::PausePlay() 
{
    m_bPause = true;
}

void CGroupDataPlayThread::ResumePlay() 
{
    m_bPause = false;
}

void CGroupDataPlayThread::StopPlay()
{
    m_bStop = true;

    wait(500);

    //if (m_pProcess)
    //{
    //    m_pProcess->OnStopPlay();
    //}
}

bool CGroupDataPlayThread::OnPlaying(int nOffset)
{
    if (!m_pGroupManager)
    {
        return false;
    }

    m_nCurrentTime += nOffset;//进来时时间已经+1了
    if (m_nCurrentTime >= m_nCurrentCompareTime)
    {
        if (m_nPastIndex >= m_pGroupManager->GetActionCount()-1)
        {
            // 所有块都已经执行完
            //m_bStop = true;
            //StopPlay();
            //if (m_pProcess)
            //{
            //    m_pProcess->OnStopPlay();
            //}
            return false;
        }
        else
        {
            // 执行下一块数据
            m_pCurrentActionBlockPlay = m_pGroupManager->GetAt(++m_nPastIndex);
            m_nCurrentTime = 0;
            m_nCurrentCompareTime = m_pCurrentActionBlockPlay->GetRunTime() + m_pCurrentActionBlockPlay->GetStopTime();
            if (m_pProcess)
            {
                bool bEnd = (m_nPastIndex == m_pGroupManager->GetActionCount() - 1) ? true : false;
                m_pProcess->ProcessPlayActionBlock(m_pCurrentActionBlockPlay, m_pCurrentActionBlockPlay->GetRunTime()*m_nScaleTime, m_nScaleTime , bEnd);
            }
        }
    }

    return true;
}
