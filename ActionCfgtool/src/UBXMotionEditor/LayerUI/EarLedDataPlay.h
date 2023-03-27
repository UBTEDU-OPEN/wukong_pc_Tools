#ifndef EARLEDDATAPLAY_H
#define EARLEDDATAPLAY_H

#include "motioneditor_global.h"
#include "IGroupDataPlay.h"
#include "ActionBlockData.h"
#include "LayerUI/ILayerUI.h"
#include "ActionBlockDataManager.h"
#include "GroupDataPlayThread.h"
#include "UpdateTimeLineProcess.h"
//#include "IUBXRobot3DView.h"
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class MOTIONEDITOR_EXPORT CEarPlayProcess: public QObject
{
    Q_OBJECT
public:
    CEarPlayProcess(IUILayerManager* pLayerManager);
    ~CEarPlayProcess(void);

public:
    void PlayProcess(int RunTime, int LeftLed, int RightLed, int Bright, int LedLightUpTime, int LedLightDownTime, bool bEnd);

signals:
    void SigEarLedPlay(int RunTime, int LeftLed, int RightLed, int Bright, int LedLightUpTime, int LedLightDownTime, bool bEnd);
};

class EarLedDataPlay: public CGroupDataPlayThread, public CActionBlockProcess
{
public:
    EarLedDataPlay(IUILayerManager* pLayerManager, int nGroupStartTime);
    ~EarLedDataPlay(void);

protected:
    ILayerOptionHandler* m_pOptionWidget;
    int m_runTime;
    int m_stopTime;
    char m_LeftLed;
    char m_RightLed;
    char m_LedBright;
    int m_LedLight;
    int m_LedTurnOff;
    int m_CurrentTime;
    int m_TimeScale;

    CEarPlayProcess* m_pPlayProcess;
//    CUpdateTimeLineProcess* m_pUpdateProcess;    //当仿真时更新时间线的位置

private:
    void StartImitationLed();

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

#endif // EARLEDDATAPLAY_H
