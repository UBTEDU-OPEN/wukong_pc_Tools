#include "stable.h"
#include "MusicDataPlay.h"
#include "UILayerManager.h"
#include <QDir>
#include "util.h"
#include "configs.h"
#include <QDebug>

CMp3PlayProcess::CMp3PlayProcess(IUILayerManager* pLayerManager)
{
    m_strMusicPath = "";
    m_pMP3Player = NULL;
    m_pLayerManager = pLayerManager;

    m_pMP3Player = new MP3Player();
    connect(this, SIGNAL(sigFilePath(QString)), m_pMP3Player, SLOT(slotFilePath(QString)));
    QObject::connect(this, &CMp3PlayProcess::SigPlayMp3, m_pMP3Player, &MP3Player::slotOnPlayMp3);
    connect(this, SIGNAL(sigStop()), m_pMP3Player, SLOT(slotStop()));
    connect(this, SIGNAL(sigRelease()), m_pMP3Player, SLOT(slotRelease()));

//    QObject::connect(m_pMP3Player, &MP3Player::SigPositionChanged, (CUILayerManager*)m_pLayerManager->GetWidget(), &CUILayerManager::OnMp3PositionChange);
}

CMp3PlayProcess::~CMp3PlayProcess(void)
{
    // 先异步释放QMediaPlayer资源 , 再释放内存
    sigRelease();
    SAFE_DELETE(m_pMP3Player);
}

void CMp3PlayProcess::SetMp3FilePath(QString strPath)
{
    m_strMusicPath = strPath;
}

void CMp3PlayProcess::OnResumeMP3()
{
    if(m_pMP3Player)
    {
        m_pMP3Player->Resume();
    }
}

void CMp3PlayProcess::OnStopMP3()
{
    // Modify by jianjie 2017/12/01 释放QMediaPlayer 资源
    //emit sigStop();
    emit sigRelease();
}

void CMp3PlayProcess::OnPauseMP3()
{
    if(m_pMP3Player)
    {
        m_pMP3Player->Pause();
    }
}

void CMp3PlayProcess::OnPlayMP3(int nStartTime , int nTotalTime)
{
    //if (m_pMP3Player)
    //{
    //    m_pMP3Player->SetFileName(m_strMusicPath);
    //}
    emit sigFilePath(m_strMusicPath);
    emit SigPlayMp3(nStartTime , nTotalTime);
}

CMusicDataPlay::CMusicDataPlay(IUILayerManager* pUILayerManager)
{
    m_pMP3PlayProcess = NULL;
    m_strMusicPath = "";
//    m_bStop = false;
    m_pMP3PlayProcess = new CMp3PlayProcess(pUILayerManager);
}

CMusicDataPlay::~CMusicDataPlay(void)
{
    SAFE_DELETE(m_pMP3PlayProcess);
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
void CMusicDataPlay::ProcessPlayActionBlock(CActionBlockData* pActionBlock, int nRunTime, int TimeScale, bool bEnd)
{
    //从音乐数据块中获取要仿真音乐文件的全路径
    if (NULL == pActionBlock)
    {
        return;
    }

    WCHAR* pwstrName = pActionBlock->GetDescription();
    //QString strMusicName = QString::fromUtf16(pwstrName);
    QString strMusicName = QString::fromStdWString(pwstrName);
    m_strMusicPath = CConfigs::getUserDirPath(USERDIR_MUSIC) + QDir::separator() + strMusicName;

    // 注意时间折算为ms
    int nEndTime = pActionBlock->GetRunTime() * TimeScale;
    int nStartTime = nEndTime - nRunTime;

    if (m_pMP3PlayProcess)
    {
        m_pMP3PlayProcess->SetMp3FilePath(m_strMusicPath);
        m_pMP3PlayProcess->OnPlayMP3(nStartTime , nEndTime);
    }
}

void CMusicDataPlay::OnStopPlay()
{
    if (m_pMP3PlayProcess)
    {
        m_pMP3PlayProcess->OnStopMP3();
    }
}

void CMusicDataPlay::StartPlay(int nEndTime, void* PlayHandle, int nBlockIndex, int nTimeOffset)
{
//  m_optionWidget = (ILayerOptionHandler*)PlayHandle;
    SetProcessHander(this);
    CGroupDataPlayThread::StartPlay(nEndTime, NULL , nBlockIndex , nTimeOffset);
}

void CMusicDataPlay::PausePlay()
{
    if (m_pMP3PlayProcess)
    {
        m_pMP3PlayProcess->OnPauseMP3();
    }
}

void CMusicDataPlay::ResumePlay()
{
    if (m_pMP3PlayProcess)
    {
        m_pMP3PlayProcess->OnResumeMP3();
    }
}

void CMusicDataPlay::StopPlay()
{
    qDebug() << "music data stop" << endl;
    if (m_pMP3PlayProcess)
    {
        m_pMP3PlayProcess->OnStopMP3();
    }

    CGroupDataPlayThread::StopPlay();
}
