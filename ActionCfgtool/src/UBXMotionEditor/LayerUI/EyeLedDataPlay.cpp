#include "EyeLedDataPlay.h"
#include <QElapsedTimer>
#include "OptionEyeLedWidget.h"
#include "util.h"

CEyePlayProcess::CEyePlayProcess(IUILayerManager* pLayerManager)
{
    //QObject::connect(this, &CEyePlayProcess::SigEyeLedPlay, (CUILayerManager*)pLayerManager->GetWidget(), &CUILayerManager::slotEyeLedPlay);
    QObject::connect(this , SIGNAL(SigEyeLedPlay(int, int, int, int, int, int, int, bool)), 
        (CUILayerManager*)pLayerManager->GetWidget(), SLOT(slotEyeLedPlay(int, int, int, int, int,int,int, bool)));
}

CEyePlayProcess::~CEyePlayProcess(void)
{
}

void CEyePlayProcess::PlayProcess(int LeftLed, int RightLed, int Bright, int Color, int runTime, int LightUpTime, int LightDownTime, bool bEnd)
{
    emit SigEyeLedPlay(LeftLed, RightLed, Bright, Color, runTime , LightUpTime, LightDownTime, bEnd);
}

EyeLedDataPlay::EyeLedDataPlay(IUILayerManager* pLayerManager, int nGroupStartTime)
{
    m_pPlayProcess = new CEyePlayProcess(pLayerManager);
//    m_pUpdateProcess = new CUpdateTimeLineProcess(pLayerManager, nGroupStartTime);
    m_pUILayerManager = pLayerManager;
}

EyeLedDataPlay::~EyeLedDataPlay(void)
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
void EyeLedDataPlay::ProcessPlayActionBlock(CActionBlockData* pActionBlock, int nRunTime, int TimeScale, bool bEnd)
{
    if (pActionBlock == NULL)
    {
        return;
    }
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
    // 颜色
    UBX_MEMCPY_INC(m_LedColor, pData);
    // 亮的时间
    UBX_MEMCPY_INC(m_LedLight, pData);
    // 灭的时间
    UBX_MEMCPY_INC(m_LedTurnOff, pData);

    // 实物仿真
    if (m_pPlayProcess)
    {
        m_pPlayProcess->PlayProcess(m_LeftLed, m_RightLed, m_LedBright, m_LedColor, nRunTime, m_LedLight*TimeScale, m_LedTurnOff*TimeScale, bEnd);
    }

    // 软件仿真
    COptionEyeLedWidget* eyeWidget = (COptionEyeLedWidget*)m_pOptionWidget;
    eyeWidget->ImitationEyeLed(nRunTime/TimeScale, m_LeftLed, m_RightLed, m_LedBright, m_LedColor, m_LedLight, m_LedTurnOff);

    delete []tempData;

//    int nTotalTime = pActionBlock->GetRunTime() + pActionBlock->GetStopTime();
//    m_pUpdateProcess->OnUpdateTimeLine(nTotalTime);
}

void EyeLedDataPlay::OnStopPlay()
{
    if (m_pUILayerManager)
    {
        m_pUILayerManager->IUILayerManager_StopPlayEyeData();
    }
}

void EyeLedDataPlay::StartPlay(int nEndTime, void* PlayHandle, int nBlockIndex, int nTimeOffset)
{
    m_pOptionWidget = (ILayerOptionHandler*)PlayHandle;
    SetProcessHander(this);
    CGroupDataPlayThread::StartPlay(nEndTime, m_pOptionWidget , nBlockIndex , nTimeOffset);
}

void EyeLedDataPlay::StopPlay()
{
    CGroupDataPlayThread::StopPlay();
}
