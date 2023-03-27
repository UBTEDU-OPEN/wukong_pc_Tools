#pragma once
#include "motioneditor_global.h"
#include "GroupDataPlayThread.h"
//#include "ActionBlockData.h"
#include "mp3player.h"

#define MAX_PATH_SIZE 500

class MOTIONEDITOR_EXPORT CMp3PlayProcess: public QObject
{
    Q_OBJECT
public:
//    explicit CMp3PlayProcess(QObject *parent = 0);
    CMp3PlayProcess(IUILayerManager* pLayerManager);
    ~CMp3PlayProcess(void);

public:
    void OnPlayMP3(int nStartTime , int nTotalTime);
    void OnResumeMP3();
    void OnPauseMP3();
    void OnStopMP3();
//    void SetInitalPama(QString strPath, IUILayerManager* pLayerManager);
    void SetMp3FilePath(QString strPath);

signals:
    void sigFilePath(QString strFilePath);

    void SigPlayMp3(quint64 startMilliSecs, quint64 endMilliSecs);

    void sigStop();

    void sigRelease();

private:
    MP3Player* m_pMP3Player;
    QString m_strMusicPath;
    IUILayerManager* m_pLayerManager;
};

class CMusicDataPlay: public CGroupDataPlayThread, public CActionBlockProcess , public QObject
{
public:
    CMusicDataPlay(IUILayerManager* pUILayerManager);
	~CMusicDataPlay(void);

private:
    QString m_strMusicPath;               //待仿真音乐的全路径
    CMp3PlayProcess* m_pMP3PlayProcess;   //音乐仿真处理器
    bool m_bStop;

public:
    //CGroupDataPlayThread 接口重写
    virtual void StartPlay(int nEndTime, void* PlayHandle, int nBlockIndex = 0, int nTimeOffset = 1);
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
    virtual void ProcessPlayActionBlock(CActionBlockData* pActionBlock, int nRunTime, int TimeScale, bool bEnd = false);
    virtual void OnStopPlay();
    virtual void StopPlay();
    virtual void PausePlay();
    virtual void ResumePlay();
};

