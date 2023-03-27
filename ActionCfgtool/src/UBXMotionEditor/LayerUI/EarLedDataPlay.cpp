#include "EarLedDataPlay.h"
#include <QElapsedTimer>
#include "OptionsEarLedWidget.h"
#include "util.h"

CEarPlayProcess::CEarPlayProcess(IUILayerManager* pLayerManager)
{
    connect(this, SIGNAL(SigEarLedPlay(int, int, int, int, int, int, bool)), 
        (CUILayerManager*)pLayerManager->GetWidget(), SLOT(slotEarLedPlay(int, int, int, int, int, int, bool)));
}

CEarPlayProcess::~CEarPlayProcess(void)
{
}

void CEarPlayProcess::PlayProcess(int RunTime, int LeftLed, int RightLed, int Bright, int LedLightUpTime, int LedLightDownTime , bool bEnd)
{
    emit SigEarLedPlay(RunTime, LeftLed, RightLed, Bright, LedLightUpTime, LedLightDownTime, bEnd);
}
//-----------------------------------------------------------------

EarLedDataPlay::EarLedDataPlay(IUILayerManager* pLayerManager, int nGroupStartTime)
{
    m_pPlayProcess = new CEarPlayProcess(pLayerManager);
    m_pUILayerManager = pLayerManager;
}

EarLedDataPlay::~EarLedDataPlay(void)
{
    SAFE_DELETE(m_pPlayProcess);
//    SAFE_DELETE(m_pUpdateProcess);
}

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
void EarLedDataPlay::ProcessPlayActionBlock(CActionBlockData* pActionBlock, int nRunTime, int TimeScale , bool bEnd)
{
    m_TimeScale = TimeScale;
    m_runTime = pActionBlock->GetRunTime()*m_TimeScale;
    m_stopTime = pActionBlock->GetStopTime();

    char* pData = NULL;
    int nLen = 0;
    pActionBlock->GetExtBlockData(&pData, nLen);
    char* tempData = pData;

    int nTotalLen = 0;
    UBX_MEMCPY_INC(nTotalLen, pData);
    if (nTotalLen != nLen)
        return;

    // 左眼LED
    UBX_MEMCPY_INC(m_LeftLed, pData);
    // 右眼LED
    UBX_MEMCPY_INC(m_RightLed, pData);
    // 亮度
    UBX_MEMCPY_INC(m_LedBright, pData);
    // 亮的时间
    UBX_MEMCPY_INC(m_LedLight, pData);
    // 灭的时间
    UBX_MEMCPY_INC(m_LedTurnOff, pData);
    delete []tempData;

    // 实物仿真
    if (m_pPlayProcess)
    {
        m_pPlayProcess->PlayProcess(m_runTime, m_LeftLed, m_RightLed, m_LedBright, m_LedLight*m_TimeScale, m_LedTurnOff*m_TimeScale , bEnd);
    }

    // 软件仿真
    COptionsEarLedWidget* earWidget = (COptionsEarLedWidget*)m_pOptionWidget;
    earWidget->ImitationEarLed(nRunTime/TimeScale, m_LeftLed, m_RightLed, m_LedBright, m_LedLight, m_LedTurnOff);
//    int nTotalTime = pActionBlock->GetRunTime() + pActionBlock->GetStopTime();
//    m_pUpdateProcess->OnUpdateTimeLine(nTotalTime);
}

void EarLedDataPlay::OnStopPlay()
{
    if (m_pUILayerManager)
    {
        m_pUILayerManager->IUILayerManager_StopPlayEarData();
    }
}

void EarLedDataPlay::StartPlay(int nEndTime, void* PlayHandle, int nBlockIndex, int nTimeOffset)
{
    m_pOptionWidget = (ILayerOptionHandler*)PlayHandle;
    SetProcessHander(this);
    CGroupDataPlayThread::StartPlay(nEndTime, m_pOptionWidget , nBlockIndex , nTimeOffset);
}

void EarLedDataPlay::StopPlay()
{
    CGroupDataPlayThread::StopPlay();
}

