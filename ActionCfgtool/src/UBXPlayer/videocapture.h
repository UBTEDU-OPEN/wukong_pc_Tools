/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：CVideoCapture
* 创建时间：20160830
* 文件标识：
* 文件摘要：video capture img of video file
*
* 当前版本：1.0.0.0
* 作    者：asu
*/

#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H

#include <QMediaPlayer>
#include <QVideoProbe>
#include <QVideoWidget>
#include <QEventLoop>
#include <QAbstractVideoSurface>
#include <QGraphicsItem>

class CVideoItem
    : public QAbstractVideoSurface,
      public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    explicit CVideoItem(QGraphicsItem *parentItem = 0);
    ~CVideoItem();

public:
    QRectF boundingRect() const;


    //video surface
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    bool start(const QVideoSurfaceFormat &format);
    void stop();
    bool present(const QVideoFrame &frame);

signals:
    void sigVideoFrame(const QVideoFrame &videoFrame);


private:
    QImage::Format imageFormat;
    QSize imageSize;

    QVideoFrame currentFrame;
    bool framePainted;
};


class CVideoCapture : public QObject
{
    Q_OBJECT

public:
    CVideoCapture(QObject *parent = 0);
    ~CVideoCapture();

public:
    //capture the video frame
    bool captureFrame(const QString& strVideoPath,
                      const QString& strImgPath,
                      const int nCaptureFrame,
                      const int nOuttimeMsec = 50000);

signals:
    void sigCaptureFrame(bool bCapture);

public slots:
    bool processFrame(QVideoFrame);

protected slots:
    void onTimeout();

    // 为了规避QT5.5.1版本直接在play后设置静音，在Mac系统下不起作用的Bug
    void slotOnVideoAutioAvailableChanged(bool bAvailable);

    // 通过播放视频的方式获取视频的第一帧画面，但是由于QMediaPlayer默认在播放前需要加载整个视频文件，但是
    // 实际上可能只需要加载5%（mac下获取不到缓冲进度）的时候播放就能捕获到第一帧的视频画面，因此采用定时
    // 方式，每间隔一定时间，先把视频暂停后再播放的方式，加快捕获视频第一帧画面的进度
    void slotOnTryCaptureTime();

private:
    bool convertYUV420ToRGB32(const uchar *yuv,int width,int height,uchar *&rgb,int &realLen);			//!YUV420转RGB32 公式算法
    bool convertNV12ToYUV420P(uchar *pNV12,int width,int height, uchar *&pOutYuv420p,int &outLen);		//!NV12转换YUV420P算法

private:
    QEventLoop* m_pEventLoop; //事件循环

    CVideoItem* m_pVideoItem;//for mac, must have to new one because of m_mediaPlayer
    QVideoWidget *m_pVideoWidget;
    QVideoProbe *m_pVideoProbe;

    QMediaPlayer m_mediaPlayer;

    QString m_strImgPath;
    QString m_strVideoPath;
    int m_nCaptureFrame;
    int m_nCurrentFrame;

    bool m_bOkSetProbeToMediaPlayer;
    bool m_bCaptureDone; // 视频捕捉结束
};

#endif
