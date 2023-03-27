/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：mp3player.h
* 创建时间：2016/01/12
* 文件标识：
* 文件摘要：mp3播放类，用于播放MP3并通知外部mp3播放的位置
*
* 当前版本：1.0.0.0
* 作    者：hels
* 完成时间：2016/01/12
* 版本摘要：
*/


#ifndef MP3PLAYER_H
#define MP3PLAYER_H
#include "motioneditor_global.h"
#include <QObject>
#include <QMediaPlayer>
#include "LayerUI/IUILayerManager.h"

class MOTIONEDITOR_EXPORT MP3Player : public QObject
{
    Q_OBJECT
public:
    MP3Player();
    ~MP3Player();

    /************************************
    * 名称: SetFileName
    * 功能: 设置mp3文件路径
    * 参数: [in]const QString& mp3FileName MP3文件路径
    * 返回:   void
    * 时间:   2016/01/12
    * 作者:   hels
    ************************************/
    void SetFileName(const QString mp3FileName);

    void SetLayerManager(IUILayerManager* pLayerManager);

    /************************************
    * 名称: Play
    * 功能: 播放mp3
    * 参数: [in]quint64 startMilliSecs 开始时间
    * 参数: [in]quint64 endMilliSecs 结束时间
    * 返回:   void
    * 时间:   2016/01/12
    * 作者:   hels
    ************************************/
    void Play(quint64 startMilliSecs, quint64 endMilliSecs);

    bool IsPlaying();
    void Pause();
    void Resume();

public slots:
    void slotFilePath(QString strFilePath);

    void slotOnPlayMp3(quint64 startMilliSecs, quint64 endMilliSecs);

    void slotStop();

    void slotRelease();

signals:
    //hels MP3播放时，通知外部播放的时间
    void SigPositionChanged(quint64 position);
    //hels 完成播放时通知外部
    void SigFinishPlaying();

private slots:
    //hels 监听QMediaPlayer信号，获取持续时间和当前播放位置等信息
    void OnDurationChanged(qint64 duration);
    void OnPositionChanged(qint64 position);
    //void OnStateChanged(QMediaPlayer::State state);
    void slotError(QMediaPlayer::Error error);

    void slotStatus(QMediaPlayer::MediaStatus status);

private:
    QMediaPlayer* m_pMediaPlayer;//可通过setPlaylist支持多个列表播放，目前暂时只有一个
    QString m_strFileName;
    IUILayerManager* m_pLayerManager;
    bool m_bPause;
    bool m_bStop;
    //播放的起止时间，若m_nEndMilliSecs不指定(默认=0)，则默认播放至结束
    quint64 m_nStartMilliSecs;
    quint64 m_nEndMilliSecs;
    quint64 m_nTotalDuration;//音乐总时长
    bool m_bIsSchedulePlay;//mp3的播放是异步的，调用play后只是Shcedule了一次播放，
                            //此时去调用SetPosition是无效的，导致mp3总是从0开始播放，因此添加记录
};

#endif // MP3PLAYER_H
