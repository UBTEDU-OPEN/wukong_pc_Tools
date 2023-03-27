/*********************************************
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：ubxvideoplayer.h
* 创建时间：2016/08/22 18:20
* 文件标识：
* 文件摘要：播放器控件
*
* 当前版本：1.0.0.0
* 作    者：hwx
* 完成时间：2016/08/22 18:20
* 版本摘要：
*/

#ifndef UBXVIDEOPLAYER_H
#define UBXVIDEOPLAYER_H

#include <QWidget>

#include <QMediaPlayer>
#include <QVideoWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsVideoItem>

#include "ubxplayer_global.h"
#include "videoplayercontrlwidget.h"
#include "ubxplayerconst.h"
#include "ubxvideooutputitem.h"

class CVideoCapture;

using namespace UBXPlayerConstants;

namespace Ui {
class UBXVideoPlayer;
}

class UBXPLAYERSHARED_EXPORT UBXVideoPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit UBXVideoPlayer(QWidget *parent = 0);
    ~UBXVideoPlayer();

    /************************************
    * 名称: loadVideoFile
    * 功能: 加载视频文件
    * 参数: [in] strFile视频文件
    * 返回:   void
    * 时间:   2016/08/22
    * 作者:   hwx
    ************************************/
    bool loadVideoFile(const QString &strFile);

    bool setImage(const QString &strImageFile);

    /************************************
    * 名称: resetVideoPlayer
    * 功能: 复位播放器
    * 返回:   void
    * 时间:   2016/08/27
    * 作者:   hwx
    ************************************/
    void resetVideoPlayer();

    /************************************
    * 名称: getPlayer
    * 功能: 获取播放器指针
    * 返回:   指向播放器的指针
    * 时间:   2016/08/22
    * 作者:   hwx
    ************************************/
    QMediaPlayer *getPlayer() { return m_pMediaPlayer; }

signals:
    /************************************
    * 名称: sigOnVideoLoaded
    * 功能: 视频文件加载后的信号
    * 返回:   void
    * 时间:   2016/08/22
    * 作者:   hwx
    ************************************/
    void sigOnVideoLoaded();

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void hideEvent(QHideEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private slots:
    /************************************
    * 名称: slotOnPlay
    * 功能: 视频播放的槽函数
    * 返回:   void
    * 时间:   2016/08/22
    * 作者:   hwx
    ************************************/
    void slotOnPlay();

    /************************************
    * 名称: slotOnPause
    * 功能: 视频暂停播放
    * 返回:   void
    * 时间:   2016/08/22
    * 作者:   hwx
    ************************************/
    void slotOnPause();

    /************************************
    * 名称: slotOnStop
    * 功能: 视频停止播放的槽函数
    * 返回:   void
    * 时间:   2016/08/22
    * 作者:   hwx
    ************************************/
    void slotOnStop();

    /************************************
    * 名称: slotOnVolumeChanged
    * 功能: 视频声音调节的槽函数
    * 参数: [in] nValue: 声音值
    * 返回:   void
    * 时间:   2016/08/22
    * 作者:   hwx
    ************************************/
    void slotOnVolumeChanged(int nValue);

    /************************************
    * 名称: slotOnUpdatePlayTime
    * 功能: 更新视频播放进度
    * 参数: [in] nValue: 播放时间
    * 返回:   void
    * 时间:   2016/08/22
    * 作者:   hwx
    ************************************/
    void slotOnUpdatePlayTime(quint64 nValue);

    /************************************
    * 名称: slotOnVideoPlayStateChanged
    * 功能: 视频播放器播放状态发生变化的槽函数
    * 参数: [in] newState: 当前播放状态
    * 返回:   void
    * 时间:   2016/10/08
    * 作者:   hwx
    ************************************/
    void slotOnVideoPlayStateChanged(QMediaPlayer::State newState);

    /************************************
    * 名称: slotOnMediaStatusChanged
    * 功能: 视频状态发生变化的槽函数
    * 参数: [in] newState: 当前状态
    * 返回:   void
    * 时间:   2016/10/12
    * 作者:   hwx
    ************************************/
    void slotOnMediaStatusChanged(QMediaPlayer::MediaStatus newStatus);

    /************************************
    * 名称: slotOnBufferStatusChanged
    * 功能: 视频缓存状态变化的槽函数
    * 参数: [in] nPercentFilled: 缓存百分比
    * 返回:   void
    * 时间:   2016/09/06
    * 作者:   hwx
    ************************************/
    void slotOnBufferStatusChanged(int nPercentFilled);

    /************************************
    * 名称: slotOnMousePressed
    * 功能: 点击视频播放界面的槽函数
    * 返回:   void
    * 时间:   2016/10/09
    * 作者:   hwx
    ************************************/
    void slotOnMousePressed();

private:
    /************************************
    * 名称: setUpUI
    * 功能: 构建界面
    * 返回:   void
    * 时间:   2016/08/22
    * 作者:   hwx
    ************************************/
    void setUpUI();

    /************************************
    * 名称: init
    * 功能: 初始化
    * 返回:   void
    * 时间:   2016/08/25
    * 作者:   hwx
    ************************************/
    void init();

    /************************************
    * 名称: updateSceneRect
    * 功能: 更新场景矩形区
    * 返回:   void
    * 时间:   2016/10/14
    * 作者:   hwx
    ************************************/
    void updateSceneRect();

    /************************************
    * 名称: addPlayItem
    * 功能: 添加播放item
    * 返回:   void
    * 时间:   2016/10/11
    * 作者:   hwx
    ************************************/
    void addPlayItem();

    /************************************
    * 名称: updatePlayItem
    * 功能: 更新播放按钮显示的位置
    * 返回:   void
    * 时间:   2016/10/11
    * 作者:   hwx
    ************************************/
    void updatePlayItem();

    /************************************
    * 名称: removePlayItem
    * 功能: 移除播放item
    * 返回:   void
    * 时间:   2016/10/11
    * 作者:   hwx
    ************************************/
    void removePlayItem();

    /************************************
    * 名称: getPlayState
    * 功能: 获取字符串格式的播放状态，方便打印
    * 返回:   播放状态
    * 时间:   2016/10/28
    * 作者:   hwx
    ************************************/
    QString getPlayState(QMediaPlayer::State eState);

    /************************************
    * 名称: getMediaStatus
    * 功能: 获取字符串格式的媒体状态
    * 返回:   媒体状态
    * 时间:   2016/10/28
    * 作者:   hwx
    ************************************/
    QString getMediaStatus(QMediaPlayer::MediaStatus eStatus);

    /************************************
    * 名称: addVideoPreviewFrameItem
    * 功能: 添加视频预览图片
    * 返回:   void
    * 时间:   2016/10/11
    * 作者:   hwx
    ************************************/
    void addVideoPreviewFrameItem();

    /************************************
    * 名称: updateVideoPreviewFrameItem
    * 功能: 更新视频预览图片帧大小和位置
    * 返回:   void
    * 时间:   2016/10/11
    * 作者:   hwx
    ************************************/
    void updateVideoPreviewFrameItem();

    /************************************
    * 名称: removeVideoPreviewFrame
    * 功能: 移除视频预览图片
    * 返回:   void
    * 时间:   2016/10/11
    * 作者:   hwx
    ************************************/
    void removeVideoPreviewFrame();

    void removeDescImageItem();

    void showVideoControl(bool bShow);

    /************************************
    * 名称: adjustShowOrder
    * 功能: 调整显示顺序，预览图片在播放图片前显示
    * 返回:   void
    * 时间:   2016/10/12
    * 作者:   hwx
    ************************************/
    void adjustShowOrder();

    /************************************
    * 名称: setDefaultStyle
    * 功能: 设置窗体默认样式
    * 返回:   void
    * 时间:   2016/08/25
    * 作者:   hwx
    ************************************/
    void setDefaultStyle();

private:
    bool  m_bIsPlayVideo;  // 是否播放视频

    QMediaPlayer *m_pMediaPlayer;
     VideoPlayerContrlWidget *m_pVideoPlayContrlWidget;

    UBXVideoOutputItem *m_pVideoGraphicsItem; // 视频播放器输出item
    QGraphicsScene *m_pGraphicsScene; // 场景
    QGraphicsView *m_pGraphicsView; // 视图

    QGraphicsPixmapItem *m_pGraphPixmapPlayItem; // 视频播放item
    QGraphicsPixmapItem *m_pGraphpixmapPreviewFrameItem; // 视频预览图片帧
    QGraphicsPixmapItem *m_pGraphpixmapDescImageItem;  // 动作详情图片
private:
    Ui::UBXVideoPlayer *ui;
};

#endif // UBXVIDEOPLAYER_H
