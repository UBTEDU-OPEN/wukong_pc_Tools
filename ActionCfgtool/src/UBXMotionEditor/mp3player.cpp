#include "mp3player.h"
#include "UILayerManager.h"
#include <QDebug>

MP3Player::MP3Player()
{
    m_nStartMilliSecs = 0;
    m_nEndMilliSecs = 0;
    m_bStop = true;
    m_bPause = false;

    m_pMediaPlayer = NULL;
    m_bIsSchedulePlay = false;
}

MP3Player::~MP3Player()
{
    // Delete by jianjie 2017/11/29
    //m_bStop = true;
    //if (m_pMediaPlayer)
    //{
    //    delete m_pMediaPlayer;
    //    m_pMediaPlayer = NULL;
    //}
}

void MP3Player::slotRelease()
{
    if (m_pMediaPlayer)
    {
        m_pMediaPlayer->stop();
        delete m_pMediaPlayer;
        m_pMediaPlayer = NULL;
    }
}

void MP3Player::slotFilePath(QString strFilePath)
{
    m_strFileName = strFilePath;

    if (NULL == m_pMediaPlayer)
    {
        m_pMediaPlayer = new QMediaPlayer();

        connect(m_pMediaPlayer, &QMediaPlayer::durationChanged, this, &MP3Player::OnDurationChanged);
        connect(m_pMediaPlayer, &QMediaPlayer::positionChanged, this, &MP3Player::OnPositionChanged);

        connect(m_pMediaPlayer, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(slotError(QMediaPlayer::Error)));

        connect(m_pMediaPlayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(slotStatus(QMediaPlayer::MediaStatus)));

        //hels 设置position等改变时，发送通知的时间间隔，经测试不能设置为1，
        //否则会按默认的1000即1s来通知，具体支持到多小精度，没有一一试过,10ms已经够用
        m_pMediaPlayer->setNotifyInterval(500);
    }
    

    if (m_pMediaPlayer)
    {
        m_pMediaPlayer->setMedia(QUrl::fromLocalFile(m_strFileName));
        m_pMediaPlayer->setVolume(50);
    }
}

void MP3Player::SetFileName(const QString mp3FileName)
{
    m_strFileName = mp3FileName;

    if (m_pMediaPlayer)
    {
        m_pMediaPlayer->setMedia(QUrl::fromLocalFile(m_strFileName));
        m_pMediaPlayer->setVolume(50);
    }
}

void MP3Player::SetLayerManager(IUILayerManager* pLayerManager)
{
    m_pLayerManager = pLayerManager;
}

void MP3Player::slotOnPlayMp3(quint64 startMilliSecs, quint64 endMilliSecs)
{
    Play(startMilliSecs, endMilliSecs);
}

void MP3Player::Play(quint64 startMilliSecs, quint64 endMilliSecs)
{
    m_nStartMilliSecs = startMilliSecs;
    m_nEndMilliSecs = endMilliSecs;

    if (m_pMediaPlayer)
    {
        m_pMediaPlayer->setPosition(startMilliSecs);
        m_pMediaPlayer->play();
    }
    
    m_bIsSchedulePlay = true;
    m_bPause = false;
    m_bStop = false;
}

bool MP3Player::IsPlaying()
{
    if(m_bStop || m_bPause)
    {
        return false;
    }
    return true;
}

void MP3Player::Pause()
{
    if(m_pMediaPlayer)
    {
        m_pMediaPlayer->pause();
    }
    m_bPause = true;
}

void MP3Player::Resume()
{
    if(m_pMediaPlayer)
    {
        m_pMediaPlayer->play();
    }
    m_bPause = false;
}

void MP3Player::slotStop()
{
    if(m_pMediaPlayer)
    {
        m_pMediaPlayer->stop();
    }
    m_bStop = true;
}

void MP3Player::slotError(QMediaPlayer::Error error)
{
    QString str = m_pMediaPlayer->errorString();
    qDebug() << error << str << endl;
}

void MP3Player::slotStatus(QMediaPlayer::MediaStatus status)
{
    qDebug() << "media status:" << status << endl;
}

void MP3Player::OnDurationChanged(qint64 duration)
{
    m_nTotalDuration = duration;
}

void MP3Player::OnPositionChanged(qint64 position)
{
    if(m_nStartMilliSecs > m_nEndMilliSecs)
    {
        m_bStop = true;

        if (m_pMediaPlayer)
        {
            m_pMediaPlayer->stop();
        }

        emit SigFinishPlaying();
        return;
    }

    if(NULL == m_pMediaPlayer)
        return;
    if(m_pMediaPlayer->state() != QMediaPlayer::PlayingState)
        return;
    //已经试过多种方法，QMediaplay的异步播放方式，导致setPosition后还是从0开始，
    //因此这里先使用这种方式，监测到这种情况时，重新设置一次
    if(m_bIsSchedulePlay && position < m_nStartMilliSecs)
    {
        m_pMediaPlayer->pause();
        m_pMediaPlayer->setPosition(m_nStartMilliSecs);
        m_pMediaPlayer->play();
        return;
    }
    m_bIsSchedulePlay = false;
    emit SigPositionChanged(position);
    if(position >= m_nEndMilliSecs)
    {
        if(!m_bStop && m_pMediaPlayer)
        {
            m_bStop = true;
            m_pMediaPlayer->stop();
            emit SigFinishPlaying();
        }
    }
}

