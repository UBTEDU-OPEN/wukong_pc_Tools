#ifndef EYELEDDATAPLAY_H
#define EYELEDDATAPLAY_H

#include "motioneditor_global.h"
#include "IGroupDataPlay.h"
#include "ActionBlockData.h"
#include "LayerUI/ILayerUI.h"
#include "ActionBlockDataManager.h"
#include "GroupDataPlayThread.h"
#include "UpdateTimeLineProcess.h"
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class MOTIONEDITOR_EXPORT CEyePlayProcess: public QObject
{
    Q_OBJECT
public:
    CEyePlayProcess(IUILayerManager* pLayerManager);
    ~CEyePlayProcess(void);

public:
    void PlayProcess(int LeftLed, int RightLed, int Bright, int Color, int runTime, int LightUpTime, int LightDownTime, bool bEnd);

signals:
    void SigEyeLedPlay(int LeftLed, int RightLed, int Bright, int Color, int runTime, int LightUpTime, int LightDownTime, bool bEnd);
};

class EyeLedDataPlay: public CGroupDataPlayThread, public CActionBlockProcess
{
public:
    EyeLedDataPlay(IUILayerManager* pLayerManager, int nGroupStartTime);
    ~EyeLedDataPlay(void);

protected:
    ILayerOptionHandler* m_pOptionWidget;
    int m_runTime;
    int m_stopTime;
    char m_LeftLed;
    char m_RightLed;
    char m_LedBright;
    char m_LedColor;
    int m_LedLight;
    int m_LedTurnOff;
    int m_CurrentTime;
    int m_TimeScale;

    CEyePlayProcess* m_pPlayProcess;
//    CUpdateTimeLineProcess* m_pUpdateProcess;    //当仿真时更新时间线的位置

public:
    //CGroupDataPlayThread 接口重写
    virtual void StartPlay(int nEndTime, void* PlayHandle, int nBlockIndex = 0, int nTimeOffset = 1);

public:
    // CActionBlockProcess 接口实现
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
    virtual void ProcessPlayActionBlock(CActionBlockData* pActionBlock, int nTotalTime, int TimeScale, bool bEnd);
    virtual void OnStopPlay();
    virtual void StopPlay();
};

#endif // EYELEDDATAPLAY_H
