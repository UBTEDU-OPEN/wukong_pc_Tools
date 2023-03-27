#include "stable.h"
#include "GroupDataManagerPlay.h"
#include "ILayerUI.h"
#include "ActionBlockDataManager.h"
#include "MotionDataPlay.h"
#include "EyeLedDataPlay.h"
#include "EarLedDataPlay.h"
#include "MusicDataPlay.h"
#include "ExpressionDataPlay.h"
#include "util.h"


bool CreateIGroupDataManagerPlay(IGroupDataManagerPlay** ppManagerPlay)
{
    CGroupDataManagerPlay* pProxy = new CGroupDataManagerPlay();
    if (pProxy)
    {
        *ppManagerPlay = pProxy;
        return true;
    }

    return false;
}

//仿真时更新时间线，转发更新信号类
//-----------------------------------------------------------------
CUpdateProcess::CUpdateProcess(IUILayerManager* pLayerManager)
{
    QObject::connect(this, &CUpdateProcess::SigUpdateTimeLine, (CUILayerManager*)pLayerManager->GetWidget(), &CUILayerManager::slotUpdateTimeLine);
}

CUpdateProcess::~CUpdateProcess(void)
{
}

void CUpdateProcess::CUpdateTimeLine(quint64 nPosition, int nType)
{
    emit SigUpdateTimeLine(nPosition, nType);
}
//-----------------------------------------------------------------

CGroupDataManagerPlay::CGroupDataManagerPlay(void)
    : m_bSinglePlay(true)
    , m_bPausePlay(false)
    , m_pLayer(NULL)
    , m_pEvent(NULL)
    , m_pCurrentPlayGroupManager(NULL)
    , m_nCurrentPlayIndex(0)
    , m_nCurrentConsumeTime(0)
    , m_nGroupEndTime(0)
    , m_nGroupStartTime(0)
    , m_bIsCompleted(false)
    , m_bStop(false)
    , m_nCurrentCol(1)
    , m_pGroupDataPlay(NULL)
    , m_PlayHandle(NULL)
    , m_pLayerManager(NULL)
    , m_pSocketSession(NULL)
    , m_nLayerType(0)
    , m_pUpdateProcess(NULL)
{

}

CGroupDataManagerPlay::~CGroupDataManagerPlay(void)
{
    if (m_pGroupDataPlay != NULL)
    {
        m_pGroupDataPlay->ReleaseConnection();
        m_pGroupDataPlay = NULL;
    }
    //ReleaseConnection();
    SAFE_DELETE(m_pUpdateProcess);
}

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
bool CGroupDataManagerPlay::Connection(ILayerUI* pDataManager, IGroupDataManagerPlay_Event* pEvent, void* PlayHandle, bool bSingleLayer)
{
    m_bStop = false;
    m_PlayHandle = PlayHandle;
    m_pLayer = pDataManager;
    m_pEvent = pEvent;
    m_bSinglePlay = bSingleLayer;
    m_nCurrentPlayIndex = 0;

    m_pGroupDataPlay = NULL;
    m_nGroupStartTime = 0;

    if (m_pLayer)
    {
        m_nLayerType  = m_pLayer->GetType();
        m_nCurrentCol = m_pLayer->GetCurCol();
    }
    m_nCurrentConsumeTime = m_nCurrentCol;

    // 创建块数据仿真对象，(对象创建一次，线程可多次启动)
    if (   m_pLayer->GetType() == ACTION_LAYER_MOTION
        || m_pLayer->GetType() == ACTION_LAYER_HAND
        || m_pLayer->GetType() == ACTION_LAYER_LEG
        || m_pLayer->GetType() == ACTION_LAYER_HEAD)
    {
        m_pGroupDataPlay = new CMotionDataPlay(m_pLayerManager, m_nGroupStartTime);
    }

    if (m_pLayer->GetType() == ACTION_LAYER_EAR)
    {
        m_pGroupDataPlay = new EarLedDataPlay(m_pLayerManager, m_nGroupStartTime);
    }

    if (m_pLayer->GetType() == ACTION_LAYER_EYE)
    {
        m_pGroupDataPlay = new EyeLedDataPlay(m_pLayerManager, m_nGroupStartTime);
    }

    if (m_pLayer->GetType() == ACTION_LAYER_MUSIC && NULL == m_pGroupDataPlay)
    {
        m_pGroupDataPlay = new CMusicDataPlay(m_pLayerManager);
    }

    if (m_pLayer->GetType() == ACTION_LAYER_EXPRESSION)
    {
        m_pGroupDataPlay = new CExpressionDataPlay(m_pLayerManager);
    }

    return true;
}

void CGroupDataManagerPlay::ReleaseConnection()
{
    if (m_pGroupDataPlay)
    {
        m_pGroupDataPlay->ReleaseConnection();
        m_pGroupDataPlay = NULL;
    }

    m_bStop = true;
    wait(1000);

    delete this;
}

void CGroupDataManagerPlay::ReSetState()
{
    if (m_pLayer)
    {
        m_pLayer->ResetCurCol();
    }

    m_nCurrentConsumeTime = 0;
    m_nCurrentPlayIndex = 0;
    m_nCurrentCol = 0;
    m_bIsCompleted = false;
    m_bPausePlay = false;
    m_bStop = false;
}

void CGroupDataManagerPlay::run()
{
    if (m_bSinglePlay == false)
        return;

    while (m_bStop == false)
    {
        if (m_bIsCompleted)
            break;

        if (m_bPausePlay)
            continue;

        if (m_pLayer->GetScaleValue() < 0)
        {
            break;
        }
        msleep(m_pLayer->GetScaleValue());

        // 如果此时已经停止了，就没有必要继续仿真了
        if (!OnPlaying(1))
        {
            break;
        }
    }

    //if (m_pGroupDataPlay)
    //{
    //    m_pGroupDataPlay->StopPlay();
    //}

    ReSetState();
}

bool CGroupDataManagerPlay::OnPlaying(int nOffsetTime)
{
    m_nCurrentConsumeTime += nOffsetTime;

    m_pUpdateProcess->CUpdateTimeLine(m_nCurrentConsumeTime, m_nLayerType);

    if (m_nCurrentConsumeTime > m_nGroupEndTime)
    {
        // 没有下一组数据，直接返回
        if (!GetNextGroupPlay())
        {
            return false;
        }
    }

    if (m_nCurrentConsumeTime == m_nGroupStartTime + 1)
    {
        if (m_pGroupDataPlay)
        {
            m_pGroupDataPlay->StartPlay(m_nGroupEndTime - m_nGroupStartTime, m_PlayHandle);
        }
    }

    return true;
}

void CGroupDataManagerPlay::StartPlay()
{
    // Add by jianjie 2017/11/23 定位当前组，从当前组开始仿真
    int nGroupIndex = -1;
    int nActionIndex = -1;
    int nTimeOffset = -1;
    m_pLayer->GetSelectGroupAction(m_nCurrentCol, nGroupIndex, nActionIndex, nTimeOffset);
    if (-1 != nActionIndex)
    {
        m_nCurrentPlayIndex = nGroupIndex;
    }
    else
    {
        m_nCurrentPlayIndex = nGroupIndex + 1;
    }

    if (GetNextGroupPlay())
    {
        if (m_nCurrentConsumeTime >= m_nGroupStartTime)
        {
            if (m_pGroupDataPlay)
            {
                m_pGroupDataPlay->StartPlay(m_nGroupEndTime-m_nGroupStartTime, m_PlayHandle , nActionIndex , nTimeOffset);
            }
        }
    }

    m_bStop = false;
    if (m_bSinglePlay)
    {
        start();
    }
}

bool CGroupDataManagerPlay::GetNextGroupPlay()
{
    if (m_pLayer == NULL || m_pLayer->GetBlockDataManager()->GetCount() == 0)
    {
        m_bIsCompleted = true;
        m_pEvent->LayerDebugStop(m_pLayer->GetType());

        // Delete by jianjie 2017/11/03 此处停止没必要，播放线程已结束，而且音乐播放跨线程停止有bug
        //if (m_pGroupDataPlay)
        //{
        //    m_pGroupDataPlay->StopPlay();
        //}
        return false;
    }

    if (m_nCurrentPlayIndex >= m_pLayer->GetBlockDataManager()->GetCount()){
        m_pCurrentPlayGroupManager = NULL;
        m_bIsCompleted = true;
        m_bStop = true;
        m_pEvent->LayerDebugStop(m_pLayer->GetType());
        //if (m_pGroupDataPlay)
        //{
        //    m_pGroupDataPlay->StopPlay();
        //}
        return false;
    }

    m_pCurrentPlayGroupManager = m_pLayer->GetBlockDataManager()->GetAt(m_nCurrentPlayIndex++);
    if (NULL == m_pCurrentPlayGroupManager)
    {
        return false;
    }

    m_nGroupEndTime = m_pCurrentPlayGroupManager->GetEndTime();
    m_nGroupStartTime = m_pCurrentPlayGroupManager->GetStartTime();

    if (m_pGroupDataPlay)
    {
        //m_pGroupDataPlay->StopPlay();
        m_pGroupDataPlay->Connect(m_pCurrentPlayGroupManager, true, m_pLayer->GetScaleValue());
    }

    return true;
}

void CGroupDataManagerPlay::StopPlay()
{
    m_bStop = true;
    wait(1000);

    if (m_pGroupDataPlay)
    {
        m_pGroupDataPlay->StopPlay();

        if (m_pEvent && m_pLayer)
        {
            m_pEvent->LayerDebugStop(m_pLayer->GetType());
        }
    }

}

void CGroupDataManagerPlay::OnResumePlay(emActionLayerType nType)
{
    m_bPausePlay = false;

    if (m_pGroupDataPlay)
    {
        m_pGroupDataPlay->ResumePlay();
    }
}

void CGroupDataManagerPlay::PausePlay(emActionLayerType nType)
{
    m_bPausePlay = true;

    if (m_pGroupDataPlay)
    {
        m_pGroupDataPlay->PausePlay();
    }
}

//void CGroupDataManagerPlay::IGroupDataManagerPlay_SetRobot3DView(IUBXRobot3DView* pSession)
//{
//    m_pRobot3dView = pSession;
//}

void CGroupDataManagerPlay::IGroupDataManagerPlay_SetLayerManager(IUILayerManager* pLayerManager)
{
    m_pLayerManager = pLayerManager;
    m_pUpdateProcess = new CUpdateProcess(m_pLayerManager);
}

/**************************************************************************
* 函数名：IGroupDataManagerPlay_SetSocketSession
* 功能: 设置Socket对象，以便与机器人通信
* 参数:
*    @[in ] pSession: Socket对象指针
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CGroupDataManagerPlay::IGroupDataManagerPlay_SetSocketSession(CIUBXSocketSession *pSession)
{
    m_pSocketSession = pSession;
}
