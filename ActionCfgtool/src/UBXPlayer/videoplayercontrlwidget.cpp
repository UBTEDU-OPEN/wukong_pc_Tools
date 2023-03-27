/*********************************************
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：videoplayercontrlwidget.h
* 创建时间：2016/08/22 18:20
* 文件标识：
* 文件摘要：播放器控制控件
*
* 当前版本：1.0.0.0
* 作    者：hwx
* 完成时间：2016/08/25 18:20
* 版本摘要：
*/

#include <QDebug>
#include <QTime>
#include <QCursor>

#include "videoplayercontrlwidget.h"
#include "ui_videoplayercontrlwidget.h"
#include "ubxvideoplayer.h"

VideoPlayerContrlWidget::VideoPlayerContrlWidget(QWidget *parent) :
    QWidget(parent),
    m_pParentWidget(NULL),
    m_pTimeSlider(NULL),
    m_pVolumeSlider(NULL),
    m_nLastPlayPosition(0),
    m_nPlayPosNotChangedCnt(0),
    ui(new Ui::VideoPlayerContrlWidget)
{
    ui->setupUi(this);

    m_pParentWidget = (UBXVideoPlayer *) parent;

    setUpUI();
    initUI();
}

VideoPlayerContrlWidget::~VideoPlayerContrlWidget()
{
    delete ui;
}

/************************************
* 名称: reset
* 功能: 复位播放器控件
* 返回:   void
* 时间:   2016/08/27
* 作者:   hwx
************************************/
void VideoPlayerContrlWidget::reset()
{
    resetButtons();
    m_nLastPlayPosition = 0;
    m_nPlayPosNotChangedCnt = 0;
    setDurationPlayTime(0);
}

/************************************
* 名称: slotOnVideoLoaded
* 功能: 视频文件加载后的槽函数
* 返回:   void
* 时间:   2016/08/22
* 作者:   hwx
************************************/
void VideoPlayerContrlWidget::slotOnVideoLoaded()
{
    setButtoneState(QMediaPlayer::StoppedState);
}

/************************************
* 名称: slotOnPositionChanged
* 功能: 视频当前播放位置变化的槽函数
* 参数: [in] nPos: 当前播放位置, 单位ms
* 返回:   void
* 时间:   2016/08/22
* 作者:   hwx
************************************/
void VideoPlayerContrlWidget::slotOnPositionChanged(quint64 nPos)
{
    m_pTimeSlider->setValue(nPos);
    m_nLastPlayPosition = nPos;

    qDebug() << "Position changed: " << nPos << endl;
//    qDebug() << "Buffer percent: " << m_pParentWidget->getPlayer()->bufferStatus() << endl;

    if (!m_pParentWidget)
    {
        return;
    }

    QMediaPlayer *pPlayer = m_pParentWidget->getPlayer();
    if (!pPlayer)
    {
        return;
    }

    // Mac系统下停止播放后positionChanged竟然还会不断触发！！！！！
    // 你妹的
    if  (pPlayer->state() != QMediaPlayer::PlayingState)
    {
        return;
    }

    //////////////////////////////////////////////////////////////
    // 由于Qt在Mac系统下存在bug，文件播放结束后
    // 不一定会马上触发EndofMedia状态变化，因此
    // 在这里认为当当前位置大于等于视频长度-允许误差便宜值
    // 时，认为播放结束
    ////////////////////////////////////////////////////////////
#ifdef Q_OS_MAC
    quint64 nDuration = pPlayer->duration();
    if (nPos >= (nDuration - MEIDA_PLAY_END_OFF_SET))
    {
        slotOnStopClicked();
    }
    else
    {
        // 由于在Mac系统下部分视频缓冲很慢，播放的时候会很卡
        // 因此当发现播放的位置没有发生变化时，暂停播放后再开始
        // 播放，可以缓解该问题
        if ((nPos > 0) && (nPos == m_nLastPlayPosition))
        {
            // 如果连续N次位置没有变化，则先暂停后播放
            if (m_nPlayPosNotChangedCnt < UBXPlayerConstants::PLAY_POSITION_NOT_CHANGED_MAX_CNT)
            {
                m_nPlayPosNotChangedCnt++;
            }
            else
            {
                pPlayer->pause();
                pPlayer->play();
                m_nPlayPosNotChangedCnt = 0;
                qDebug() << "Video player pause and play" << endl;
            }
        }
        else
        {
            m_nPlayPosNotChangedCnt = 0;
        }
    }

#endif
}

/************************************
* 名称: slotOnDurationChanged
* 功能: 视频时长变化的槽函数
* 参数: [in] nDuration: 视频时长, 单位ms
* 返回:   void
* 时间:   2016/08/22
* 作者:   hwx
************************************/
void VideoPlayerContrlWidget::slotOnDurationChanged(quint64 nDuration)
{
    qDebug() << "Duration changed: " << nDuration << endl;
    setDurationPlayTime(nDuration);
    m_pTimeSlider->setRange(0, nDuration);
}

/************************************
* 名称: doOnPlayStateChanged
* 功能: 处理播放状态发生变化
* 参数: [in] eState: 当前播放状态
* 返回:   void
* 时间:   2016/08/22
* 作者:   hwx
************************************/
void VideoPlayerContrlWidget::doOnPlayStateChanged(QMediaPlayer::State eState)
{
    setButtoneState(eState);
}

/************************************
* 名称: doOnMediaStatusChanged
* 功能: 处理媒体状态发生变化
* 参数: [in] eStatus: 当前媒体状态
* 返回:   void
* 时间:   2016/10/12
* 作者:   hwx
************************************/
void VideoPlayerContrlWidget::doOnMediaStatusChanged(QMediaPlayer::MediaStatus eStatus)
{
    switch (eStatus) {
    case QMediaPlayer::LoadingMedia:
    {
        break;
    }
    case QMediaPlayer::LoadedMedia:
    {
#ifdef Q_OS_MAC
        // Mac系统下反复加载同一个视频文件时，不会触发durationChanged()信号
        // 导致视频时长不会设置
        QMediaPlayer *pPlayer = m_pParentWidget->getPlayer();
        if (pPlayer)
        {
            slotOnDurationChanged(pPlayer->duration());
        }
#endif
        break;
    }
    case QMediaPlayer::StalledMedia:
    {
        break;
    }
    case QMediaPlayer::BufferingMedia:
    {
        break;
    }
    case QMediaPlayer::BufferedMedia:
    {
        break;
    }
    case QMediaPlayer::EndOfMedia:
    {
//        setPlayButtonState(true);
//        setStopButtonState(true);

//        ui->pbVideoPause->hide();
//        ui->pbVideoPlay->show();
        slotOnStopClicked();
        break;
    }
    default:
        break;
    }
}

/************************************
* 名称: slotOnPlayClicked
* 功能: 视频播放
* 返回:   void
* 时间:   2016/08/22
* 作者:   hwx
************************************/
void VideoPlayerContrlWidget::slotOnPlayClicked()
{
//    setButtoneState(eStateVideoPlayerPlay);
    emit sigOnPlay();
}

/************************************
* 名称: slotOnPauseClicked
* 功能: 视频播放暂停
* 返回:   void
* 时间:   2016/08/22
* 作者:   hwx
************************************/
void VideoPlayerContrlWidget::slotOnPauseClicked()
{
//    setButtoneState(eStateVideoPlayerPause);
    emit sigOnPause();
}

/************************************
* 名称: slotOnStopClicked
* 功能: 视频停止播放
* 返回:   void
* 时间:   2016/08/22
* 作者:   hwx
************************************/
void VideoPlayerContrlWidget::slotOnStopClicked()
{
//    setButtoneState(eStateVideoPlayerStop);
    m_pTimeSlider->setValue(0);
    emit sigOnStop();
}

/************************************
* 名称: slotOnTimeSliderMoved
* 功能: 移动播放进度的槽函数
* 参数: [in] nValue: 当前进度值
* 返回:   void
* 时间:   2016/08/25
* 作者:   hwx
************************************/
void VideoPlayerContrlWidget::slotOnTimeSliderMoved(quint64 nValue)
{
    emit sigOnUpdatePlayTime(nValue);
}

/************************************
* 名称: slotOnTimeSliderPressed
* 功能: 点击播放进度的槽函数
* 返回:   void
* 时间:   2016/08/25
* 作者:   hwx
************************************/
void VideoPlayerContrlWidget::slotOnTimeSliderPressed()
{
    // emit sigOnUpdatePlayTime(ui->horizontalTimeSlider->value());
}

/************************************
* 名称: slotOnVolumnButtonToggle
* 功能: 声音图标按下的槽函数
* 返回:   void
* 时间:   2016/11/25
* 作者:   hwx
************************************/
void VideoPlayerContrlWidget::slotOnVolumnButtonToggle(bool bChecked)
{
    if (!m_pParentWidget)
    {
        return;
    }

    QMediaPlayer *pPlayer = m_pParentWidget->getPlayer();
    if (!pPlayer)
    {
        return;
    }

    pPlayer->setMuted(bChecked);

    if (bChecked)
    {
        ui->tbVolume->setStyleSheet("background-color: transparent;"
                                                            "qproperty-icon: url(:/res/images/icon_volume_mute.png);"
                                                            "qproperty-iconSize: 24px 24px;");
    }
    else
    {
        ui->tbVolume->setStyleSheet("background-color: transparent;"
                                                            "qproperty-icon: url(:/res/images/icon_changed_volume.png);"
                                                            "qproperty-iconSize: 24px 24px;");
    }
}

/************************************
* 名称: setUpUI
* 功能: 构建界面
* 返回:   void
* 时间:   2016/08/25
* 作者:   hwx
************************************/
void VideoPlayerContrlWidget::setUpUI()
{
    // 播放进度条
    m_pTimeSlider = new UBXSlider(this);
    m_pTimeSlider->setOrientation(Qt::Horizontal);
    ui->horizontalLayoutTimeProgress->addWidget(m_pTimeSlider);

    // 视频时长
    m_pEndTime = new QLabel(this);
    m_pEndTime->setObjectName("labelVideoPlayEndTime");
    ui->horizontalLayoutTimeProgress->addWidget(m_pEndTime);

    // 声音调节进度条
    m_pVolumeSlider = new UBXSlider(this);
    m_pVolumeSlider->setOrientation(Qt::Horizontal);
    ui->horizontalLayoutVoice->addWidget(m_pVolumeSlider);

    ui->tbVolume->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->tbVolume->setAutoRaise(true);
    ui->tbVolume->setCheckable(true);

    connect(ui->tbVolume, &QToolButton::toggled, this, &VideoPlayerContrlWidget::slotOnVolumnButtonToggle);

    connect(ui->pbVideoPlay, &QPushButton::clicked, this, &VideoPlayerContrlWidget::slotOnPlayClicked);
    connect(ui->pbVideoPause, &QPushButton::clicked, this, &VideoPlayerContrlWidget::slotOnPauseClicked);
    connect(ui->pbVideoStop, &QPushButton::clicked, this, &VideoPlayerContrlWidget::slotOnStopClicked);

    connect(m_pVolumeSlider, &QSlider::valueChanged, this, &VideoPlayerContrlWidget::sigOnVolumeChanged);

    connect(m_pTimeSlider, &QSlider::sliderMoved, this, &VideoPlayerContrlWidget::slotOnTimeSliderMoved);
    connect(m_pTimeSlider, &QSlider::sliderPressed, this, &VideoPlayerContrlWidget::slotOnTimeSliderPressed);

}

/************************************
* 名称: initUI
* 功能: 初始化界面
* 返回:   void
* 时间:   2016/08/25
* 作者:   hwx
************************************/
void VideoPlayerContrlWidget::initUI()
{
    reset();

    ui->tbVolume->setChecked(false);
    ui->tbVolume->setStyleSheet("background-color: transparent;"
                                                        "qproperty-icon: url(:/res/images/icon_changed_volume.png);"
                                                        "qproperty-iconSize: 24px 24px;");

    m_pVolumeSlider->setRange(0, PLAYER_MAX_VOLUME);
    m_pVolumeSlider->setValue(PLAYER_MAX_VOLUME/2);
}

/************************************
* 名称: resetButtons
* 功能: 复位播放器按钮
* 返回:   void
* 时间:   2016/08/25
* 作者:   hwx
************************************/
void VideoPlayerContrlWidget::resetButtons()
{
    ui->pbVideoPause->hide();
    setPlayButtonState(false);
    setPauseButtonState(false);
    setStopButtonState(false);
}

/************************************
* 名称: setPlayButtonState
* 功能: 设置播放按钮状态
* 参数: [in] bEnable是否使能
* 返回:   void
* 时间:   2016/08/25
* 作者:   hwx
************************************/
void VideoPlayerContrlWidget::setPlayButtonState(bool bEnable)
{
    ui->pbVideoPlay->setEnabled(bEnable);
}

/************************************
* 名称: setPauseButtonState
* 功能: 设置暂停按钮状态
* 参数: [in] bEnable是否使能
* 返回:   void
* 时间:   2016/08/25
* 作者:   hwx
************************************/
void VideoPlayerContrlWidget::setPauseButtonState(bool bEnable)
{
    ui->pbVideoPause->setEnabled(bEnable);
}

/************************************
* 名称: setStopButtonState
* 功能: 设置停止按钮状态
* 参数: [in] bEnable是否使能
* 返回:   void
* 时间:   2016/08/25
* 作者:   hwx
************************************/
void VideoPlayerContrlWidget::setStopButtonState(bool bEnable)
{
    ui->pbVideoStop->setEnabled(bEnable);
}

/************************************
* 名称: setButtoneState
* 功能: 设置按钮状态
* 参数: [in] eState: 播放器状态
* 返回:   void
* 时间:   2016/08/25
* 作者:   hwx
************************************/
void VideoPlayerContrlWidget::setButtoneState(QMediaPlayer::State eState)
{
    switch (eState) {
    case QMediaPlayer::PlayingState:
    {
        setPauseButtonState(true);
        setStopButtonState(true);
        setPlayButtonState(false);

        ui->pbVideoPlay->hide();
        ui->pbVideoPause->show();
        break;
    }
    case QMediaPlayer::PausedState:
    {
        setPlayButtonState(true);
        setPauseButtonState(false);
        setStopButtonState(true);

        ui->pbVideoPause->hide();
        ui->pbVideoPlay->show();
        break;
    }
    case QMediaPlayer::StoppedState:
    {
        setPlayButtonState(true);
        setStopButtonState(false);

        ui->pbVideoPause->hide();
        ui->pbVideoPlay->show();

        m_nLastPlayPosition = 0;
        m_nPlayPosNotChangedCnt = 0;
        break;
    }
    default:
        break;
    }
}

/************************************
* 名称: setDurationPlayTime
* 功能: 设置视频时长显示
* 参数: [in] nTime: 时长，单位是ms
* 返回:   void
* 时间:   2016/08/25
* 作者:   hwx
************************************/
void VideoPlayerContrlWidget::setDurationPlayTime(quint64 nTime)
{
    m_pEndTime->setText(getFormatPlayTime(nTime));
}

/************************************
* 名称: getFormatPlayTime
* 功能: 获取格式化时间显示, HH:mm:ss
* 参数: [in] nTime: 播放器状态
* 返回:   时间格式化后的字符串
* 时间:   2016/08/25
* 作者:   hwx
************************************/
QString VideoPlayerContrlWidget::getFormatPlayTime(quint64 nTime)
{
     return (QTime(0, 0, 0).addMSecs(nTime).toString(QString::fromLatin1("HH:mm:ss")));
}
