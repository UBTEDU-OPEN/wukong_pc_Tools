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

#ifndef VIDEOPLAYERCONTRLWIDGET_H
#define VIDEOPLAYERCONTRLWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMediaPlayer>

#include "ubxplayerconst.h"
#include "ubxslider.h"

class UBXVideoPlayer;

using namespace UBXPlayerConstants;

namespace Ui {
class VideoPlayerContrlWidget;
}

class VideoPlayerContrlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VideoPlayerContrlWidget(QWidget *parent);
    ~VideoPlayerContrlWidget();

    /************************************
    * 名称: reset
    * 功能: 复位播放器控件
    * 返回:   void
    * 时间:   2016/08/27
    * 作者:   hwx
    ************************************/
    void reset();

    /************************************
    * 名称: doOnPlayStateChanged
    * 功能: 处理播放状态发生变化
    * 参数: [in] eState: 当前播放状态
    * 返回:   void
    * 时间:   2016/08/22
    * 作者:   hwx
    ************************************/
    void doOnPlayStateChanged(QMediaPlayer::State eState);

    /************************************
    * 名称: doOnMediaStatusChanged
    * 功能: 处理媒体状态发生变化
    * 参数: [in] eStatus: 当前媒体状态
    * 返回:   void
    * 时间:   2016/10/12
    * 作者:   hwx
    ************************************/
    void doOnMediaStatusChanged(QMediaPlayer::MediaStatus eStatus);

public slots:
    /************************************
    * 名称: slotOnVideoLoaded
    * 功能: 视频文件加载后的槽函数
    * 返回:   void
    * 时间:   2016/08/22
    * 作者:   hwx
    ************************************/
    void slotOnVideoLoaded();

    /************************************
    * 名称: slotOnPositionChanged
    * 功能: 视频当前播放位置变化的槽函数
    * 参数: [in] nPos: 当前播放位置, 单位ms
    * 返回:   void
    * 时间:   2016/08/22
    * 作者:   hwx
    ************************************/
    void slotOnPositionChanged(quint64 nPos);

    /************************************
    * 名称: slotOnDurationChanged
    * 功能: 视频时长变化的槽函数
    * 参数: [in] nDuration: 视频时长, 单位ms
    * 返回:   void
    * 时间:   2016/08/22
    * 作者:   hwx
    ************************************/
    void slotOnDurationChanged(quint64 nDuration);

private slots:
    /************************************
    * 名称: slotOnPlayClicked
    * 功能: 视频播放
    * 返回:   void
    * 时间:   2016/08/25
    * 作者:   hwx
    ************************************/
    void slotOnPlayClicked();

    /************************************
    * 名称: slotOnPauseClicked
    * 功能: 视频播放暂停
    * 返回:   void
    * 时间:   2016/08/25
    * 作者:   hwx
    ************************************/
    void slotOnPauseClicked();

    /************************************
    * 名称: slotOnStopClicked
    * 功能: 视频停止播放
    * 返回:   void
    * 时间:   2016/08/25
    * 作者:   hwx
    ************************************/
    void slotOnStopClicked();

    /************************************
    * 名称: slotOnTimeSliderMoved
    * 功能: 移动播放进度的槽函数
    * 参数: [in] nValue: 当前进度值
    * 返回:   void
    * 时间:   2016/08/25
    * 作者:   hwx
    ************************************/
    void slotOnTimeSliderMoved(quint64 nValue);

    /************************************
    * 名称: slotOnTimeSliderPressed
    * 功能: 点击播放进度的槽函数
    * 返回:   void
    * 时间:   2016/08/25
    * 作者:   hwx
    ************************************/
    void slotOnTimeSliderPressed();

    /************************************
    * 名称: slotOnVolumnButtonToggle
    * 功能: 声音图标按下的槽函数
    * 返回:   void
    * 时间:   2016/11/25
    * 作者:   hwx
    ************************************/
    void slotOnVolumnButtonToggle(bool bChecked);

signals:
    /************************************
    * 名称: sigOnPlay
    * 功能: 视频播放
    * 返回:   void
    * 时间:   2016/08/25
    * 作者:   hwx
    ************************************/
    void sigOnPlay();

    /************************************
    * 名称: sigOnPause
    * 功能: 视频暂停播放
    * 返回:   void
    * 时间:   2016/08/25
    * 作者:   hwx
    ************************************/
    void sigOnPause();

    /************************************
    * 名称: slotOnStop
    * 功能: 视频停止播放
    * 返回:   void
    * 时间:   2016/08/25
    * 作者:   hwx
    ************************************/
    void sigOnStop();

    /************************************
    * 名称: sigOnVolumeChanged
    * 功能: 声音调节信号
    * 参数:[in] nValue: 声音值
    * 返回:   void
    * 时间:   2016/08/25
    * 作者:   hwx
    ************************************/
    void sigOnVolumeChanged(int nValue);

    /************************************
    * 名称: sigOnUpdatePlayTime
    * 功能: 播放进度变化
    * 参数:[in] nValue: 播放进度值
    * 返回:   void
    * 时间:   2016/08/25
    * 作者:   hwx
    ************************************/
    void sigOnUpdatePlayTime(quint64 nValue);

private:
    /************************************
    * 名称: setUpUI
    * 功能: 构建界面
    * 返回:   void
    * 时间:   2016/08/25
    * 作者:   hwx
    ************************************/
    void setUpUI();

    /************************************
    * 名称: initUI
    * 功能: 初始化界面
    * 返回:   void
    * 时间:   2016/08/25
    * 作者:   hwx
    ************************************/
    void initUI();

    /************************************
    * 名称: resetButtons
    * 功能: 复位播放器按钮
    * 返回:   void
    * 时间:   2016/08/25
    * 作者:   hwx
    ************************************/
    void resetButtons();

    /************************************
    * 名称: setPlayButtonState
    * 功能: 设置播放按钮状态
    * 参数: [in] bEnable是否使能
    * 返回:   void
    * 时间:   2016/08/25
    * 作者:   hwx
    ************************************/
    void setPlayButtonState(bool bEnable);

    /************************************
    * 名称: setPauseButtonState
    * 功能: 设置暂停按钮状态
    * 参数: [in] bEnable是否使能
    * 返回:   void
    * 时间:   2016/08/25
    * 作者:   hwx
    ************************************/
    void setPauseButtonState(bool bEnable);

    /************************************
    * 名称: setStopButtonState
    * 功能: 设置停止按钮状态
    * 参数: [in] bEnable是否使能
    * 返回:   void
    * 时间:   2016/08/25
    * 作者:   hwx
    ************************************/
    void setStopButtonState(bool bEnable);

    /************************************
    * 名称: setButtoneState
    * 功能: 设置按钮状态
    * 参数: [in] eState: 播放器状态
    * 返回:   void
    * 时间:   2016/08/25
    * 作者:   hwx
    ************************************/
    void setButtoneState(QMediaPlayer::State eState);

    /************************************
    * 名称: setDurationPlayTime
    * 功能: 设置视频时长显示
    * 参数: [in] nTime: 时长，单位是ms
    * 返回:   void
    * 时间:   2016/08/25
    * 作者:   hwx
    ************************************/
    void setDurationPlayTime(quint64 nTime);

    /************************************
    * 名称: getFormatPlayTime
    * 功能: 获取格式化时间显示, HH:mm:ss
    * 参数: [in] nTime: 时长，单位是ms
    * 返回:   时间格式化后的字符串
    * 时间:   2016/08/25
    * 作者:   hwx
    ************************************/
    QString getFormatPlayTime(quint64 nTime);

private:
    int m_nPlayPosNotChangedCnt; // positionChanged中播放位置一直不变的次数，此时播放卡顿，超过了指定次数需要先暂停后再播放
    quint64 m_nLastPlayPosition;  // 记录上一个postionChanged播放的位置

    QLabel *m_pEndTime;

    UBXSlider *m_pTimeSlider;
    UBXSlider *m_pVolumeSlider;

    UBXVideoPlayer *m_pParentWidget;

private:
    Ui::VideoPlayerContrlWidget *ui;
};

#endif // VIDEOPLAYERCONTRLWIDGET_H
